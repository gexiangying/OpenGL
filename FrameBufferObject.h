#pragma once
#include <GL/glew.h>
#include <vector>

class FrameBufferObject
{
public:

	FrameBufferObject();
	~FrameBufferObject();

	void attachTexture(GLenum attachment, GLuint texID, GLint level = 0);
	void detachTexture(GLenum attachment, GLint level = 0);
	void bind();
	void unBind();

private:

	void rebuildMRT();

private:

	GLuint _fbo;
	GLuint _depthRBO;
	std::vector<GLenum> _attachments;
};

