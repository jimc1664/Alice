#include "stdafx.h"

#include "Gem/Dis/Mesh.h"

#include "OGl_Task.h"

#include "Gem/String/DStr.h"




#include "Gem/ConCur/ConCur.h"

#include "Gem/Math/vec2.h"
#include "Gem/Math/vec3.h"
#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"
#include "Gem/Org/Ary.h"



struct Vertex {
  vec3f Pos;
//  vec4f colour;
  vec3f Norm;
  vec2f UV;
};

struct MeshData
{
	ary<Vertex> vertices;
	ary<int> indices;

	int getNumVerts()
	{
		return vertices.count();
	};

	int getNumIndices()
	{
		return indices.count();
	};

	~MeshData()
	{
		vertices.clear();
		indices.clear();
	}
};



void checkError();
void clearError();

Mesh::Mesh() {
	VA = VB = IB = IC = 0;
}

bool loadFBXFromFile( const CStr& filename, MeshData *meshData);

void Mesh::fromFile( DisMain &dm, const CStr &s) {

	MeshData md;
	if( loadFBXFromFile( s, &md ) ) {
		clearError();
		checkError();
#ifdef DEBUG //todo...  real reporting..
		printf("load mesh %s \n", s.str());
#endif 
		glGenVertexArrays(1, &VA);
		glBindVertexArray(VA);

		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		checkError();
		//char *d = new char[md.getNumVerts()*sizeof(Vertex)];

		//memcpy(d, &md.vertices[0], md.getNumVerts()*sizeof(Vertex));
		int err = glGetError();
		auto es = glewGetErrorString( err );
		checkError();
		int a = md.getNumVerts()*sizeof(Vertex), b = sizeof(Vertex), c = md.getNumVerts();
		glBufferData(GL_ARRAY_BUFFER, md.getNumVerts()*sizeof(Vertex), &md.vertices[0], GL_STATIC_DRAW);
				err = glGetError();
		es = glewGetErrorString( err );
		glGenBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (IC =  md.getNumIndices())*sizeof(int), &md.indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (ptr)offsetof(Vertex, Pos)  );

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (ptr)offsetof(Vertex, Norm) );

		glEnableVertexAttribArray(2);  
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (ptr)offsetof(Vertex, UV) );
	

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		checkError();
	} else Error("failed to load fbx..");
}

void Mesh::draw(RenderingCntx &rc) {
	glBindVertexArray(VA);
	glDrawElements(GL_TRIANGLES, IC, GL_UNSIGNED_INT, 0);

}


Mesh* Mesh::fromFile(const CStr &s) {

	auto ret = new Mesh;
	DStr s2 = s;

	TaskSchedule( DisMain, ret->fromFile(cntx,s2) );
	return ret;
}



#undef Error

#include <fbxsdk.h>

#pragma comment (lib, "libfbxsdk.lib") 

void processNode(FbxNode *node, MeshData *meshData);
void processAttribute(FbxNodeAttribute * attribute, MeshData *meshData);
void processMesh(FbxMesh * mesh, MeshData *meshData);
void processMeshTextureCoords(FbxMesh * mesh, Vertex * verts, int numVerts);

int level = 0;

void PrintTabs() {
	for (int i = 0; i < level; i++)
		printf("\t");
}


FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

bool loadFBXFromFile( const CStr &filename, MeshData *meshData)
{
  level = 0;
	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Create a new scene so that it can be populated by the imported file.
	if (!lImporter->Initialize(filename.str(), -1, lSdkManager->GetIOSettings()))
	{
		return false;
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	FbxGeometryConverter conv(lSdkManager);
	conv.Triangulate(lScene, true);

	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should
	// not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		//cout << "Root Node " << lRootNode->GetName() << endl;
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			processNode(lRootNode->GetChild(i),meshData);
		}
	}

	lImporter->Destroy();
	return true;
}

void processNode(FbxNode *node, MeshData *meshData)
{
	PrintTabs();
	const char* nodeName = node->GetName();
	FbxDouble3 translation =  node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	/*cout << "Node " << nodeName << " Postion " << translation[0] << " " << translation[1] << " " << translation[2] << " "
		<< " Rotation " << rotation[0] << " " << rotation[1] << " " << rotation[2] << " "
		<< " Scale " << scaling[0] << " " << scaling[1] << " " << scaling[2] << endl;
		*/
	level++;
	// Print the node's attributes.
	for (int i = 0; i < node->GetNodeAttributeCount(); i++){
		processAttribute(node->GetNodeAttributeByIndex(i),meshData);
	}

	// Recursively print the children.
	for (int j = 0; j < node->GetChildCount(); j++)
		processNode(node->GetChild(j),meshData);
	level--;
	PrintTabs();
}

void processAttribute(FbxNodeAttribute * attribute, MeshData *meshData)
{
	if (!attribute) return;
	FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
	FbxString attrName = attribute->GetName();
	//PrintTabs();
	//cout << "Attribute " << typeName.Buffer() << " Name " << attrName << endl;
	switch (attribute->GetAttributeType()) {
	case FbxNodeAttribute::eMesh: processMesh(attribute->GetNode()->GetMesh(), meshData);
	case FbxNodeAttribute::eCamera: return;
	case FbxNodeAttribute::eLight: return;
	}
}

void processMesh(FbxMesh * mesh, MeshData *meshData) {

	int numVerts = mesh->GetControlPointsCount();
	int numIndices = mesh->GetPolygonVertexCount();

	Vertex * pVerts = new Vertex[numVerts];
	int *pIndices = mesh->GetPolygonVertices();

	for (int i = 0; i < numVerts; i++)
	{
		FbxVector4 currentVert = mesh->GetControlPointAt(i);
		pVerts[i].Pos = vec3f( (f32)currentVert[0], (f32)currentVert[1], (f32)currentVert[2]);
		//pVerts[i].colour= vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		//pVerts[i].texCoords = vec2f(0.0f, 0.0f);
	}

	processMeshTextureCoords(mesh, pVerts, numVerts);

	int io = meshData->vertices.count();
	for (int i = 0; i < numVerts; i++)
	{
		meshData->vertices.add(pVerts[i]);
	}
	for (int i = 0; i < numIndices; i++)
	{
		meshData->indices.add(pIndices[i]+io);
	}
	//cout << "Vertices " << numVerts << " Indices " << numIndices << endl;


	if (pVerts)
	{
		delete[] pVerts;
		pVerts = NULL;
	}
}


void processMeshTextureCoords(FbxMesh * mesh, Vertex * verts, int numVerts)
{
	for (int iPolygon = 0; iPolygon < mesh->GetPolygonCount(); iPolygon++) {
		for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++) {
			int fbxCornerIndex = mesh->GetPolygonVertex(iPolygon, iPolygonVertex);
			FbxVector2 fbxUV = FbxVector2(0.0, 0.0);


			FbxLayerElementUV* fbxLayerUV = mesh->GetLayer(0)->GetUVs();

			// Get texture coordinate
			if (fbxLayerUV) {
				int iUVIndex = 0;
				switch (fbxLayerUV->GetMappingMode()) {
				case FbxLayerElement::eByControlPoint:
					iUVIndex = fbxCornerIndex;
					break;
				case FbxLayerElement::eByPolygonVertex:
					iUVIndex = mesh->GetTextureUVIndex(iPolygon, iPolygonVertex, FbxLayerElement::eTextureDiffuse);
					break;
				case FbxLayerElement::eByPolygon:
					iUVIndex = iPolygon;
					break;
				}
				fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);
				verts[fbxCornerIndex].UV.x = (f32)fbxUV[0];
				verts[fbxCornerIndex].UV.y = 1.0f - (f32)fbxUV[1];
			}

			//get norms here 
			FbxVector4 nrm;
			if( mesh->GetPolygonVertexNormal( iPolygon, iPolygonVertex, nrm ) ) {
				verts[fbxCornerIndex].Norm = (vec3f)vec3d(nrm[0], nrm[1], nrm[2]);
			}
		}
	}
}



