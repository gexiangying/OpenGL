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
		_model->draw(MVP_Matrix);
		glDisable(GL_DEPTH_TEST);
	}

	virtual void Setup() override {
		glm::mat4 view = glm::lookAt(glm::vec3(-150.0f, 150.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 project = glm::perspective(30.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
		MVP_Matrix = project * view;	

		_model = new Model("datas/head.fbx");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

protected:

private:

	glm::mat4 MVP_Matrix;
	Model *_model;
};