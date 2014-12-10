#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "Model.h"


class ModelImporterApp : public Application{
public:

	virtual void Render(double elapseTime) override {
		GLfloat color[] = {0.5f, 0.5f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);
		GLfloat depth[] = {1.0f};
		glClearBufferfv(GL_DEPTH, 0, depth);

		glEnable(GL_DEPTH_TEST);
		_model->draw(_viewMat);
		glDisable(GL_DEPTH_TEST);
	}

	virtual void Setup() override {
		_model = new Model("datas/teapot.obj");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

protected:

private:

	Model *_model;
};