#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "Model.h"
#include <fstream>


class AdvanceLightApp : public Application{
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

		Program *program = new Program;

		program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/WarnLighting.vert");
		program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/WarnLighting.frag");
		program->link();
		program->setUniform4f("LightPos", glm::vec4(0.0f, 50.0f, 0.0f, 1.0f));
		program->setUniform4f("LightColor", glm::vec4(1.0f));
		program->setUniformf("DiffDensity", 1.0f);
		program->setUniformf("WarnExponent", 128.0f);

		_model->setProgram(program);

	}

protected:

private:

	Model *_model;
};