#include "FrameBufferObject.h"
#include "Displayer.h"


FrameBufferObject::FrameBufferObject()
{
	glGenFramebuffers(1, &_fbo);
	// create a renderbuffer object to store depth info  
	// NOTE: A depth renderable image should be attached the FBO for depth test.  
	// If we don't attach a depth renderable image to the FBO, then  
	// the rendering output will be corrupted because of missing depth test.  
	// If you also need stencil test for your rendering, then you must  
	// attach additional image to the stencil attachement point, too.
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glGenRenderbuffers(1, &_depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Displayer::Instance()->GetWindowSize().x, Displayer::Instance()->GetWindowSize().y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBufferObject::~FrameBufferObject()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &_depthRBO);
	_depthRBO = 0;
	glDeleteFramebuffers(1, &_fbo);
}

void FrameBufferObject::attachTexture( GLenum attachment, GLuint texID, GLint level )
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, attachment, texID, level);
	_attachments.push_back(attachment);
	rebuildMRT();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::detachTexture( GLenum attachment, GLint level )
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, attachment, 0, level);
	auto iter = std::find(_attachments.begin(), _attachments.end(), attachment);
	if (iter != _attachments.end())
		_attachments.erase(iter);
	rebuildMRT();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRBO);
}

void FrameBufferObject::rebuildMRT()
{
	std::vector<GLenum> mrt;
	for (auto iter : _attachments){
		if (iter >= GL_COLOR_ATTACHMENT0 && iter <= GL_COLOR_ATTACHMENT15)
			mrt.push_back(iter);
	}

	if (!mrt.empty())
		glDrawBuffers(mrt.size(), &mrt[0]);
}

void FrameBufferObject::unBind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
