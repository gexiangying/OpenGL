//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include "TextureManager.h"

TextureManager* TextureManager::_inst(0);

TextureManager* TextureManager::Instance()
{
	if(!_inst)
		_inst = new TextureManager();

	return _inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
}

//these should never be called
//TextureManager::TextureManager(const TextureManager& tm){}
//TextureManager& TextureManager::operator=(const TextureManager& tm){}
	
TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif

	UnloadAllTextures();
	_inst = 0;
}

bool TextureManager::LoadTexture(GLuint &texID, const std::string & filename, GLint level, GLint border)
{
	for (auto iter : _textureVector){
		if (iter.file == filename){
			texID = iter.texID;
			return true;
		}
	}

	GLBITMAP *glMap = nullptr;
	FIBITMAP *dib(0);
	bool loaded = false;
	if (filename != "White"){
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename.c_str(), 0);
		//if still unknown, try to guess the file format from the file extension
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(filename.c_str());
		//if still unkown, return failure
		if(fif == FIF_UNKNOWN)
			return false;

		//check that the plugin has reading capabilities and load the file
		if(FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename.c_str());
		//if the image failed to load, return failure
		if(!dib)
			return false;

		glMap = FIBitmap2GLBitmap(dib);
		if (!glMap) return false;
		loaded = true;
	}else {
		glMap = new GLBITMAP;
		glMap->w = 2;
		glMap->h = 2;
		glMap->imgMode = glMap->internalMode = GL_RGB;
		glMap->buf = new BYTE[4 * 3];
		memset(glMap->buf, 255, 4 * 3);
	}


	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &texID);
	//store the texture ID mapping
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, glMap->internalMode, glMap->w, glMap->h, 0, glMap->imgMode, GL_UNSIGNED_BYTE, glMap->buf);

	//glTexStorage2D(GL_TEXTURE_2D, 1, glMap->internalMode, glMap->w, glMap->h);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, glMap->w, glMap->h, glMap->imgMode, GL_UNSIGNED_BYTE, glMap->buf);

	//Free FreeImage's copy of the data
	if (loaded)
		FreeImage_Unload(dib);
	else
		delete [] glMap->buf;

	FreeGLBitmap(glMap);

	TexParam tp;
	tp.file = filename;
	tp.texID = texID;
	tp.texTarget = GL_TEXTURE_2D;
	_textureVector.push_back(tp);
	//return success
	return true;
}

void TextureManager::UnloadTexture(GLuint texID)
{
	for (auto iter = _textureVector.begin(); iter != _textureVector.end(); ++iter){
		if (iter->texID == texID){
			_textureVector.erase(iter);
			break;
		}
	}
	glDeleteTextures(1, &texID);
}

void TextureManager::BindTexture(GLuint texID)
{
	for (auto iter : _textureVector){
		if (iter.texID == texID){
			//if this texture ID mapped, bind it's texture as current
			glBindTexture(iter.texTarget, iter.texID);
			return;
		}
	}
}

void TextureManager::UnloadAllTextures()
{
	//Unload the textures untill the end of the texture map is found
	for (auto iter : _textureVector){
		glDeleteTextures(1, &iter.texID);
	}
	//clear the texture map
	_textureVector.clear();
}

TextureManager::GLBITMAP * TextureManager::FIBitmap2GLBitmap( FIBITMAP *fibmp )
{
	GLBITMAP *glbmp = new GLBITMAP;
	int pitch = FreeImage_GetPitch(fibmp);  
	glbmp->buf = FreeImage_GetBits(fibmp);  
	int bpp = FreeImage_GetBPP(fibmp);  

	glbmp->w = FreeImage_GetWidth(fibmp);  
	glbmp->h = FreeImage_GetHeight(fibmp);     

	switch ( bpp ) {  
	case 8:    
		{
			glbmp->imgMode = GL_BGR;
			glbmp->internalMode = GL_RGB8;
		}
		break;  
	case 24:  
		{
			glbmp->imgMode = GL_BGR;
			glbmp->internalMode = GL_RGB;
		}
		break;  
	case 32:  
		{
			glbmp->imgMode = GL_BGRA;
			glbmp->internalMode = GL_RGBA;
		}
		break;  
	default: return NULL;     
	}  

	return glbmp;  
}

void TextureManager::FreeGLBitmap( GLBITMAP *glbmp )
{
	if ( glbmp ) {  
		delete glbmp;  
	}  
}

GLuint TextureManager::GetOrCreateSampler( GLint filterParam, GLint wrapParam )
{
	auto iter = _samplerMap.find(filterParam + wrapParam);
	if (iter != _samplerMap.end())
		return iter->second;

	GLuint smap;
	glGenSamplers(1, &smap);
	glSamplerParameteri(smap, GL_TEXTURE_MIN_FILTER, filterParam);
	glSamplerParameteri(smap, GL_TEXTURE_MAG_FILTER, filterParam);
	glSamplerParameteri(smap, GL_TEXTURE_WRAP_S, wrapParam);
	glSamplerParameteri(smap, GL_TEXTURE_WRAP_T, wrapParam);
	glSamplerParameteri(smap, GL_TEXTURE_WRAP_R, wrapParam);
	_samplerMap[filterParam + wrapParam] = smap;
	return smap;
}

GLuint TextureManager::CreateTexture( GLuint width, GLuint height, GLuint depth, GLuint internalformat, const void *data )
{
	GLuint format, type;
	getTextureType(internalformat, format, type);
	GLuint texID;
	GLuint texTarget;
	glGenTextures(1, &texID);
	if (depth <= 1){
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexStorage2D(GL_TEXTURE_2D, 1, internalformat, width, height);
		if (data){
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data);
		}
		texTarget = GL_TEXTURE_2D;
	}else{
		glBindTexture(GL_TEXTURE_3D, texID);
		glTexStorage3D(GL_TEXTURE_3D, 1, internalformat, width, height, depth);
		if (data){
			glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, format, type, data);
		}
		texTarget = GL_TEXTURE_3D;
	}

	TexParam tp;
	tp.texID = texID;
	tp.texTarget = texTarget;
	_textureVector.push_back(tp);
	return texID;
}

void TextureManager::BindSampler(GLuint texUnit, GLuint sampID)
{
	glBindSampler(texUnit, sampID);
}

void TextureManager::getTextureType( GLuint internalformat, GLuint &format, GLuint &type )
{
	switch (internalformat)
	{
	case GL_RGB32F:
		{
			format = GL_RGB;
			type = GL_FLOAT;
		}
		break;
	case GL_RGBA32F:
		{
			format = GL_RGBA;
			type = GL_FLOAT;
		}
		break;
	case GL_RGB8:
		{
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
		}
		break;
	case GL_RGBA8:
		{
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
		}
		break;
	}
}
