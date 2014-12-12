#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "Model.h"
#include <iostream>


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
		_model = new Model("datas/sphere.obj");
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		{
			Program *program = new Program;
			program->setName("WarnLighting");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/WarnLighting.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/WarnLighting.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniformf("DiffDensity", 20000.0f);
			program->setUniformf("WarnExponent", 128.0f);
			_advanceLightProgramList.push_back(program);
		}

		{
			Program *program = new Program;
			program->setName("CookTorrance");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/CookTorrance.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/CookTorrance.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniform4f("DiffColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniform4f("SpecColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniformf("Refractiable", 0.5f);
			program->setUniformf("Dm", 0.5f);
			program->setUniformf("Dc", 0.5f);
			_advanceLightProgramList.push_back(program);
		}

		{
			Program *program = new Program;
			program->setName("CookTorrance_crytek");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/CookTorrance_crytek.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/CookTorrance_crytek.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniform4f("DiffColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniform4f("SpecColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniformf("Refractiable", 0.5f);
			program->setUniformf("smoothness", 0.5f);
			_advanceLightProgramList.push_back(program);
		}

		{
			Program *program = new Program;
			program->setName("Minnaert");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/Minnaert.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/Minnaert.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniform4f("DiffColor", glm::vec4(1.0f));
			program->setUniformf("minnaertFactor", 0.8f);
			_advanceLightProgramList.push_back(program);
		}

		{
			Program *program = new Program;
			program->setName("OrenNayar");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/OrenNayar.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/OrenNayar.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniform4f("DiffColor", glm::vec4(1.0f));
			program->setUniformf("roughness", 0.5f);
			_advanceLightProgramList.push_back(program);
		}

		{
			Program *program = new Program;
			program->setName("SchlickReflection");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/SchlickReflection.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/SchlickReflection.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniform4f("DiffColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniform4f("SpecColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniformf("Shiness", 4.0f);
			_advanceLightProgramList.push_back(program);
		}

		{
			Program *program = new Program;
			program->setName("WardReflection");
			program->attachShader(GL_VERTEX_SHADER, "datas/AdvanceLighting/WardReflection.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/AdvanceLighting/WardReflection.frag");
			program->link();
			program->setUniform4f("LightPos", glm::vec4(150.0f, 150.0f, 0.0f, 1.0f));
			program->setUniform4f("LightColor", glm::vec4(1.0f));
			program->setUniform4f("DiffColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniform4f("SpecColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			program->setUniformf("roughness", 0.5f);
			program->setUniformf("roughnessx", 0.9f);
			program->setUniformf("roughnessy", 0.1f);
			program->setUniformi("isotropicOranisotropic", 1);
			_advanceLightProgramList.push_back(program);
		}

		_model->setProgram(_advanceLightProgramList[0]);
	}

	virtual void specialKeyEvent(int key, int x, int y) override {
		static int currentProgram = 0;
		bool needUpdate = false;
		if (key == GLUT_KEY_LEFT){
			--currentProgram;
			if (currentProgram < 0)
				currentProgram = _advanceLightProgramList.size() - 1;
			needUpdate = true;
		}else if (key == GLUT_KEY_RIGHT){
			++currentProgram;
			if (currentProgram >= _advanceLightProgramList.size())
				currentProgram = 0;
			needUpdate = true;
		}

		if (needUpdate){
			_model->setProgram(_advanceLightProgramList[currentProgram]);
			std::cout<<"--------------"<<_advanceLightProgramList[currentProgram]->getName()<<"--------------"<<std::endl;
		}
	}

protected:

private:

	Model *_model;
	std::vector<Program *> _advanceLightProgramList;
};