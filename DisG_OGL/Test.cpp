#include "stdafx.h"

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Texture.h"

#include "OGL_Task.h"

#include "Gem/ConCur/ConCur.h"

#include "Gem/Math/vec3.h"
#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"
#include "Gem/Org/Ary.h"

#include "Gem/Scene3/TestObj.h"
#include "Gem/Scene3/Texture.h"

#undef Error

#include <fbxsdk.h>

#pragma comment (lib, "libfbxsdk.lib") 

struct Vertex {
  vec3f position;
  vec4f colour;
  vec2f texCoords;
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

bool loadFBXFromFile( const CStr& filename, MeshData *meshData);

void processNode(FbxNode *node, MeshData *meshData);
void processAttribute(FbxNodeAttribute * attribute, MeshData *meshData);
void processMesh(FbxMesh * mesh, MeshData *meshData);
void processMeshTextureCoords(FbxMesh * mesh, Vertex * verts, int numVerts);


enum SHADER_TYPE
{
	VERTEX_SHADER=GL_VERTEX_SHADER,
	FRAGMENT_SHADER=GL_FRAGMENT_SHADER
};

GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType);
GLuint loadShaderFromFile(const CStr& filename, SHADER_TYPE shaderType);
bool checkForCompilerErrors(GLuint shaderProgram);
bool checkForLinkErrors(GLuint program);

struct Vertex2
{
  float x,y,z;  
};

Vertex2 verts[]={
	//Front Face
    {-0.5f, -0.5f, 0.5f},
     {0.5f, -0.5f, 0.5f},
     {0.5f,  0.5f, 0.5f},

	 { -0.5f, -0.5f, 0.5f },
	 {-0.5f, 0.5f, 0.5f},
	 { 0.5f, 0.5f, 0.5f },

	 //Back Face
	 { -0.5f, -0.5f, -0.5f },
	 { 0.5f, -0.5f, -0.5f },
	 { 0.5f, 0.5f, -0.5f },

	 { -0.5f, -0.5f, -0.5f },
	 { -0.5f, 0.5f, -0.5f },
	 { 0.5f, 0.5f, -0.5f },

	 //Left Face
   { -0.5f, -0.5f, -0.5f },
    { -0.5f, 0.5f, -0.5f },
    {-0.5f, -0.5f,  0.5f },

    { -0.5f, 0.5f, -0.5f },
    { -0.5f, 0.5f, 0.5f },
    { -0.5f, -0.5f, 0.5f },

	 //Right Face
   { 0.5f, -0.5f, -0.5f },
    { 0.5f, 0.5f, -0.5f },
    {0.5f, -0.5f,  0.5f },

    { 0.5f, 0.5f, -0.5f },
    { 0.5f, 0.5f, 0.5f },
    { 0.5f, -0.5f, 0.5f },

   //top Face
   {0.5f,  0.5f, 0.5f},
   {0.5f,  0.5f, -0.5f},
   {-0.5f,  0.5f, -0.5f},

   {-0.5f,  0.5f, -0.5f},
   {0.5f,  0.5f, 0.5f},
   {-0.5f,  0.5f, 0.5f},

	 //bottom Face
   {0.5f,  -0.5f, 0.5f},
   {0.5f,  -0.5f, -0.5f},
   {-0.5f,  -0.5f, -0.5f},

   {-0.5f,  -0.5f, -0.5f},
   {0.5f,  -0.5f, 0.5f},
   {-0.5f,  -0.5f, 0.5f},
   };

	  GLuint VBO = -1, IBO, VAO;
	  GLuint shaderProgram;

	  int IC;
void DrawTestCube::proc(DisMain &dm) {
//	return;


	if( VBO  == -1 ) {

		MeshData md;
		if( loadFBXFromFile( CSTR( "Media//armoredrecon.fbx" ), &md ) ) {

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

	mat4f projMatrix;
	mat4f modelMatrix;
	
	projMatrix = mat4f::projection( 45.0f *DEG_TO_RAD , 1024.0f/768.0f,0.1f, 100.0f); 
	modelMatrix = mat4f::camLookDir(  vec3f(0.0f, 0.9f, -20.0f), (vec3f(0.0f, 0.0f, 0.0f) - vec3f(0.0f, 0.9f, 20.0f)).getNormal(), vec3f(0.0f, 1.0f, 0.0f) );
	auto mvp = modelMatrix* projMatrix;
	for( int i = md.vertices.count(); i--; ) {

	//	md.vertices[i].position -= vec3f(0.0f, 0.9f, 20.0f);
		//md.vertices[i].position *= modelMatrix;

		vec4f p = vec4f( md.vertices[i].position, 1.0f ) * mvp;
		//md.vertices[i].position *= projMatrix;
		
	//	md.vertices[i].position = ((vec3f)p)/p.w;;
	}

		glBufferData(GL_ARRAY_BUFFER, md.getNumVerts()*sizeof(Vertex), &md.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (IC =  md.getNumIndices())*sizeof(int), &md.indices[0], GL_STATIC_DRAW);


	GLuint vertexShaderProgram = 0;
	CStr vsPath = CSTR("Media//Shaders//textureVS.glsl");
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);
	checkForCompilerErrors(vertexShaderProgram);

	GLuint fragmentShaderProgram = 0;
	CStr fsPath = CSTR("Media//Shaders//textureFS.glsl");
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);
	checkForCompilerErrors(fragmentShaderProgram);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);

	//Link attributes
	glBindAttribLocation(shaderProgram, 0, "vertexPosition");
	glBindAttribLocation(shaderProgram, 1, "vertexColour");
	glBindAttribLocation(shaderProgram, 2, "vertexTexCoords");

	glLinkProgram(shaderProgram);
	checkForLinkErrors(shaderProgram);
	//now we can delete the VS & FS Programs
//	glDeleteShader(vertexShaderProgram);
//	glDeleteShader(fragmentShaderProgram);


		} else {

			ThrowAssert("err");

		}
		

	}


	glUseProgram(shaderProgram);

	/*
	GLfloat projMatrix[16];
	GLfloat modelMatrix[16];
	glGetFloatv(GL_PROJECTION, projMatrix);
	glGetFloatv(GL_MODELVIEW, modelMatrix);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(projMatrix);
	glMultMatrixf(modelMatrix);
	glGetFloatv(GL_PROJECTION, projMatrix);
	glPopMatrix(); */
	

	auto projMatrix = mat4f::projection( 45.0f * DEG_TO_RAD, 1024.0f/768.0f,0.1f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto viewMatrix = mat4f::camLookAt(  vec3f(0.0f, 0.9f, -20.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f) );


	//auto viewMatrix = lookAt(vec3(0.0f, 0.0f, 20.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//auto worldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	auto MVPMatrix = Trans.as<mat4f>()* viewMatrix*projMatrix;// *worldMatrix;
	//MVPMatrix = mat4f::identity();
	//MVPMatrix = projMatrix;
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLint MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, MVPMatrix );
	GLint tex0Location = glGetUniformLocation(shaderProgram, "texture0");
	glUniform1i(tex0Location, 0 );

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3f)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3f) + sizeof(vec4f)));
	
	glActiveTexture(GL_TEXTURE0);
	Tex.Hdwr->apply(dm);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glUseProgram(0);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, IC, GL_UNSIGNED_INT, 0);


	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);

	return;

	if( VBO  == -1 ) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(Vertex2), verts, GL_STATIC_DRAW);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO );

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 30);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

//Load it from a memory buffer
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType)
{
  GLuint program = glCreateShader(shaderType);
  glShaderSource(program, 1, &pMem, NULL);
  glCompileShader(program);
  return program;
}

//Load Shader from File
GLuint loadShaderFromFile(const CStr& filename, SHADER_TYPE shaderType) {
  string fileContents;
  ifstream file;
  file.open(filename.str(), std::ios::in);
  if (!file)
  {
	  cout << "File could not be loaded" << endl;
    return 0;
  }

  //calculate file size
  if (file.good())
  {
    file.seekg(0, std::ios::end);
    u32 len = (u32)file.tellg();
    file.seekg(std::ios::beg);
    if (len == 0)
    {
      std::cout << "File has no contents " << std::endl;
      return 0;
    }

    fileContents.resize(len);
    file.read(&fileContents[0], len);
    file.close();
    GLuint program = loadShaderFromMemory(fileContents.c_str(), shaderType);
    return program;
  }

  return 0;
}

bool checkForCompilerErrors(GLuint shaderProgram)
{
  GLint isCompiled = 0;
  glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
    //The maxLength includes the NULL character
    string infoLog;
    infoLog.resize(maxLength);
    glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
    cout << "Shader not compiled " << infoLog << endl;
    //We don't need the shader anymore.
    glDeleteShader(shaderProgram);
    return true;
  }

  return false;
}

bool checkForLinkErrors(GLuint program)
{
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    //The maxLength includes the NULL character
    string infoLog;
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    cout << "Shader not linked " << infoLog << endl;
    //We don't need the shader anymore.
    glDeleteProgram(program);
    return true;
  }
  return false;
}




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
		pVerts[i].position = vec3f( (f32)currentVert[0], (f32)currentVert[1], (f32)currentVert[2]);
		pVerts[i].colour= vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		pVerts[i].texCoords = vec2f(0.0f, 0.0f);
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
				verts[fbxCornerIndex].texCoords.x = (f32)fbxUV[0];
				verts[fbxCornerIndex].texCoords.y = 1.0f - (f32)fbxUV[1];
			}
		}
	}
}


DrawTestCube::DrawTestCube( Scene3::TestObj *o ) : Tex( *o->Tex) {

	Trans.setTransformation(o->Pos, o->Rot.as<mat3f>(), o->Scale);
}
