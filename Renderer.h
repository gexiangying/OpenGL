#pragma once
#include "RenderableObject.h"
#include "glm/glm.hpp"
#include <vector>

class Camera;
class FrameBufferObject;
class Renderer
{
public:
	typedef std::vector<RenderableObject *> RenderableObjectList;

	Renderer();
	~Renderer();

	void setClearColor(const glm::vec4 &color);
	void setClearDepth(float value);
	void setCamera(Camera *camera);
	const Camera* getCamera() const { return _camera; }
	void renderToFrameBuferObject(FrameBufferObject *fbo);
	void attachRenderableObject(RenderableObject *obj);
	void detachRenderableObject(RenderableObject *obj);
	void depthTest(bool isEnable);
	void depthWrite(bool isEnable);

	virtual void render();

private:

	RenderableObjectList _renderableObjectList;
	glm::vec4 _clearColor;
	float _depthValue;
	bool _enableDepthTest;
	bool _enableDepthWrite;
	Camera *_camera;
	FrameBufferObject *_fbo;
};

