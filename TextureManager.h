//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#ifndef TextureManager_H
#define TextureManager_H

#include <GL/glew.h>
#include "FreeImage.h"
#include <map>
#include <vector>
#include <string>

class TextureManager
{
public:
	static TextureManager* Instance();
	virtual ~TextureManager();

	//load a texture an make it the current texture
	//if texID is already in use, it will be unloaded and replaced with this texture
	bool LoadTexture(GLuint &texID,			//arbitrary id you will reference the texture by
		const std::string &filename = "White",	//where to load the file from
		GLint level = 0,					//mipmapping level
		GLint border = 0);					//border size

	GLuint CreateTexture(GLuint width, GLuint height, GLuint depth, GLuint internalformat, const void *data = nullptr);

	GLuint GetOrCreateSampler(GLint filterParam, GLint wrapParam);

	//free the memory for a texture
	void UnloadTexture(GLuint texID);

	//set the current texture
	void BindTexture(GLuint texID);

	void BindSampler(GLuint texUnit, GLuint sampID);

	//free all texture memory
	void UnloadAllTextures();

protected:
	TextureManager();
	TextureManager(const TextureManager& tm);
	TextureManager& operator=(const TextureManager& tm);
	
protected:

	void generateWhileTexture();
	void getTextureType(GLuint internalformat, GLuint &format, GLuint &type);

protected:

	struct GLBITMAP
	{  
		int             w;  
		int             h;  
		BYTE           *buf;  
		GLuint          imgMode;
		GLuint          internalMode;
	};

protected:

	GLBITMAP * FIBitmap2GLBitmap(FIBITMAP *fibmp);
	void FreeGLBitmap(GLBITMAP *glbmp);

protected:

	struct TexParam
	{
		std::string file;
		GLuint texID;
		GLenum texTarget;
	};
	static TextureManager* _inst;
	std::vector<TexParam> _textureVector;
	std::map<GLint, GLuint> _samplerMap;
};

#endif