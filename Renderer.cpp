#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "Camera.h"
#include "Displayer.h"
#include "FrameBufferObject.h"

Renderer::Renderer()
	: _clearColor(0.0f, 0.0f, 0.0f, 1.0f)
	, _depthValue(1.0f)
	, _enableDepthTest(false)
	, _enableDepthWrite(true)
	, _camera(new Camera)
	, _fbo(nullptr)
{
	glm::ivec2 size = Displayer::Instance()->GetWindowSize();
	_camera->setProjectionMatrixAsOrtho(0.0f, size.x, 0.0f, size.y, 1.0f, 1000.0f);
	_camera->lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


Renderer::~Renderer()
{
}

void Renderer::attachRenderableObject( RenderableObject *obj )
{
	auto iter = std::find(_renderableObjectList.begin(), _renderableObjectList.end(), obj);
	if (iter != _renderableObjectList.end()) return;
	_renderableObjectList.push_back(obj);
}

void Renderer::detachRenderableObject( RenderableObject *obj )
{
	auto iter = std::find(_renderableObjectList.begin(), _renderableObjectList.end(), obj);
	if (iter != _renderableObjectList.end())
		_renderableObjectList.erase(iter);
}

void Renderer::render()
{
	if (_fbo)
		_fbo->bind();
	
	glViewport(0, 0, Displayer::Instance()->GetWindowSize().x, Displayer::Instance()->GetWindowSize().y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(_clearColor));
	glClearBufferfv(GL_DEPTH, 0, &_depthValue);

	if (_enableDepthTest)
		glEnable(GL_DEPTH_TEST);
	if (!_enableDepthWrite)
		glDepthMask(GL_FALSE);

	for (auto iter : _renderableObjectList){
		iter->draw(this);
	}

	if (!_enableDepthWrite)
		glDepthMask(GL_TRUE);
	if (_enableDepthTest)
		glDisable(GL_DEPTH_TEST);

	if (_fbo)
		_fbo->unBind();
}

void Renderer::setClearColor( const glm::vec4 &color )
{
	_clearColor = color;
}

void Renderer::setClearDepth( float value )
{
	_depthValue = value;
}

void Renderer::setCamera( Camera *camera )
{
	_camera = camera;
}

void Renderer::depthTest( bool isEnable )
{
	_enableDepthTest = isEnable;
}

void Renderer::depthWrite( bool isEnable )
{
	_enableDepthWrite = isEnable;
}

void Renderer::renderToFrameBuferObject( FrameBufferObject *fbo )
{
	_fbo = fbo;
}
