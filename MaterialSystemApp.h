#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "Model.h"
#include "TextureManager.h"
#include "FrameBufferObject.h"
#include "Program.h"
#include "Displayer.h"
#include "RenderQuad.h"
#include <iostream>

class MaterialSystemApp : public Application{
public:

	virtual void Render(double elapseTime) override {

		_renderer.render();
	}

	virtual void Setup() override {
		auto model = new Model("datas/sponza/sponza.obj");
		_renderer.attachRenderableObject(model);
		_renderer.setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		_renderer.depthTest(true);
	}

private:
};