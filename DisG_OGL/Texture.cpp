#include "stdafx.h"

#include "Gem/Dis/Texture.h"

#include "OGl_Task.h"

#include "Gem/String/DStr.h"

#include "../zlib/zlib.h"
#include "../libpng/png.h"

#pragma comment( lib, ExtLibName( "zlibstat" ) )
#pragma comment( lib, ExtLibName( "libpng" ) )


Texture::Texture() {

}

void checkError();



void Texture::fromFile( DisMain &dm, const CStr &s) {


	//const char* file_name = "Media//redfighter//redfighter0005.png";
	//*
	FILE *fp = 0;
	fopen_s( &fp, s, "rb");
	if (!fp) {
		Error("FileNotFound");
	}

	u8 header[8]; 	
	int number = 8;
	fread(header, 1, number, fp);

			
	bool is_png = !png_sig_cmp(header, 0, number);

	if (!is_png) {
		Error("Not Png..");
	}
	//*/
	png_structp png_ptr = png_create_read_struct
		(PNG_LIBPNG_VER_STRING, null,null,null);

	if (!png_ptr)
		Error("ERROR");

	png_infop info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr,
			(png_infopp)NULL, (png_infopp)NULL);
		Error("ERROR");
	}


	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr,
			(png_infopp)NULL);
		fclose(fp);
		Error("ERROR");
	}

	

	png_init_io(png_ptr, fp);

#include "OGlMain.h"

	png_set_sig_bytes(png_ptr, number);

	

	png_read_info(png_ptr, info_ptr);
    
	SetLastError(0);

	// Get some usefull information from header
	s32 bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	s32 color_type = png_get_color_type(png_ptr, info_ptr);
    
	// Convert index color images to RGB images
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
    
	// Convert 1-2-4 bits grayscale images to 8 bits grayscale
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);
    
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
    
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);
	else if (bit_depth < 8)
		png_set_packing(png_ptr);
    
	
	// Update info structure to apply transformations
	png_read_update_info(png_ptr, info_ptr);
		
	

	u32 width, height;
	// Retrieve updated information
	png_get_IHDR(
		png_ptr, info_ptr,
		(png_uint_32*)(&width),
		(png_uint_32*)(&height),
		&bit_depth, &color_type,
		0, 0, 0);
	

	u32 formatSize;

	GLint intFormat; GLenum format;
	switch (color_type) {
		case PNG_COLOR_TYPE_GRAY:
			format = GL_LUMINANCE;
			intFormat = GL_LUMINANCE8;
			formatSize = 1;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			format = GL_LUMINANCE_ALPHA;
			intFormat = GL_LUMINANCE8_ALPHA8;				   
			formatSize = 2;
			break;
		case PNG_COLOR_TYPE_RGB:
			format = GL_RGB;
			intFormat = GL_RGB8;	
			formatSize = 3;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			format = GL_RGBA;
			intFormat = GL_RGBA8;	
			formatSize = 4;
			break;
		default:
			Error("Invalid color type in PNG file");
			break;
	}


	auto imageBuffer = new u8[ width * height * formatSize ];
	auto row_pointers = new png_bytep[height];

  
    
	for (u32 i = 0; i < height; ++i) {
		row_pointers[i] =
			(png_bytep)(imageBuffer +
			( i * width * formatSize) );
	}
    
	// Read pixel data using row pointers
	png_read_image(png_ptr, row_pointers);
    
	// Finish decompression and release memory
	png_read_end(png_ptr, 0);
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);

	fclose(fp);
    
		

	// We don't need row pointers anymore
	delete [] row_pointers;

	
	gen(dm, imageBuffer, width, height, intFormat, format);

	delete [] imageBuffer;

	//png_set_compression_buffer_size(png_ptr, buffer_size);
	/*
	void read_row_callback(png_structp png_ptr,
		png_uint_32 row, int pass);
	{
		// put your code here
	}

	(You can give it another name that you like instead of "read_row_callback")

	To inform libpng about your function, use

	png_set_read_status_fn(png_ptr, read_row_callback); */
	/*
	png_set_gamma(png_ptr, PNG_DEFAULT_sRGB, PNG_DEFAULT_sRGB);
	png_set_alpha_mode(png_ptr, PNG_ALPHA_PNG, PNG_DEFAULT_sRGB);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);


	auto row_pointers = png_get_rows(png_ptr, info_ptr);
			
	/*
	width            = png_get_image_width(png_ptr,
					info_ptr);

	height           = png_get_image_height(png_ptr,
					info_ptr);

	bit_depth        = png_get_bit_depth(png_ptr,
					info_ptr);

	color_type       = png_get_color_type(png_ptr,
					info_ptr);

	interlace_type   = png_get_interlace_type(png_ptr,
					info_ptr);

	compression_type = png_get_compression_type(png_ptr,
					info_ptr);

	filter_method    = png_get_filter_type(png_ptr,
					info_ptr);

	channels = png_get_channels(png_ptr, info_ptr);

	channels       - number of channels of info for the
				color type (valid values are 1 (GRAY,
				PALETTE), 2 (GRAY_ALPHA), 3 (RGB),
				4 (RGB_ALPHA or RGB + filler byte))

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	* /


	auto width            = png_get_image_width(png_ptr, info_ptr);

	auto height           = png_get_image_height(png_ptr, info_ptr); 

	*/


}


void checkError();

void Texture::gen(DisMain &dm, ptr imageBuffer, u32 width, u32 height, u32 intFormat, u32 format ){

	
	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);
        
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 	
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(
        GL_TEXTURE_2D, 0, intFormat, width, height,
        0, format, GL_UNSIGNED_BYTE, imageBuffer
    );

	glGenerateMipmap(GL_TEXTURE_2D);

	//
	checkError();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, TexID);
}


void Texture::apply(DisMain &dm ) {
	glBindTexture(GL_TEXTURE_2D, TexID);
}


Texture* Texture::fromFile(const CStr &s) {
	auto ret = new Texture;
	DStr s2 = s;

	TaskSchedule( DisMain, ret->fromFile(cntx,s2) );
	/*
	
	auto lambda = [=](DisMain &cntx) { 
		ret->fromFile(cntx, s2); 
	}; 
		typedef decltype(lambda) LambdaT; 
		TaskHlpr<LambdaT, DisMain>::ctor(lambda);
	int a = 0;	
	
	struct FromFile_Texture : public OGL_Task<FromFile_Texture> {  //todo - this is all wrong...

		~FromFile_Texture() {
			int a = 0;
		}
		void proc(DisMain &dm) { Tex.fromFile(dm, Str);  }
		Texture &Tex;
		DStr Str;
	//private: friend class OGL_Task<FromFile_Texture>;
		FromFile_Texture( Texture* t, const CStr &s ) : Tex( *t ), Str(s) {}
	};
	FromFile_Texture::ctor( ret, s ); */

	return ret;
}

/*
void Camera::setOutput( DisMain &dm, RenderTarget &rt ) {
	rt.add(dm, *this);
}

Camera::Camera( ) {

}
void Camera::render(DisMain &dm) {

	//render();
	//DrawList 
	

}
void Camera::setOutput( RenderTarget &rt ) {
	struct SetOutput_Camera : public OGL_Task<SetOutput_Camera> {
		void proc( DisMain &dm ) { Cam.setOutput(dm,RT); }
		RenderTarget &RT;
		Camera &Cam;
	private: friend class OGL_Task<SetOutput_Camera>;
		SetOutput_Camera( Camera* c, RenderTarget* rt ) : Cam( *c ), RT(*rt) {}
	};
	SetOutput_Camera::ctor( this, &rt );
}*/