#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "TextureManager.h"
#include "FrameBufferObject.h"
#include "Program.h"
#include "Displayer.h"
#include "RenderQuad.h"
#include "SphereShape.h"
#include "BoxShape.h"
#include "CylinderShape.h"
#include "CapsuleShape.h"
#include "ShapeRenderable.h"
#include "Material.h"
#include <iostream>

static const GLchar *V_Shader[] = {
	"#version 430                                                    \n"
	"layout (location = 0) in vec3 position;                        \n"
	"layout (location = 1) in vec3 normal;                          \n"
	"layout (location = 2) in vec4 texcoord;                         \n"
	"layout (location = 3) in vec4 color;                         \n"
	"uniform mat4 u_ModelViewProjectMat;                              \n"
	"uniform mat4 u_ModelViewMat;                                     \n"
	"uniform mat3 u_NormalMat;                                       \n"
	"out vec3 v_position;                                             \n"
	"out vec3 v_normal;                                             \n"
	"out vec2 v_texcoord;                                             \n"
	"out vec4 v_color;                                             \n"
	"void main()                                                     \n"
	"{                                                               \n"
	"	v_normal = u_NormalMat * normal;                              \n"
	"	v_position = (u_ModelViewMat * vec4(position, 1.0)).xyz;         \n"
	"	v_texcoord = texcoord.xy;                             \n"
	"	v_color = color;                             \n"
	"	gl_Position = u_ModelViewProjectMat * vec4(position, 1.0);                 \n"
	"}                                                               \n"
};

static const GLchar *F_Shader[] = {
	"#version 430                        \n"
	"uniform vec4 u_diffuseColor;                                       \n"
	"uniform vec4 u_specularColor;                                       \n"
	"uniform vec4 u_ambientColor;                                       \n"
	"uniform float u_shininess;                                       \n"
	"uniform sampler2D u_diffuseTex;                                       \n"
	"in vec3 v_position;                                             \n"
	"in vec3 v_normal;                                             \n"
	"in vec2 v_texcoord;                                             \n"
	"in vec4 v_color;                                             \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	vec3 lDir = normalize(vec3(1.0, 1.0, 1.0));                       \n"
	"	vec3 normal = normalize(v_normal);                        \n"
	"	vec3 eye = normalize(-v_position);                       \n"
	"	vec3 H = normalize(eye + lDir);                       \n"
	"	float specfactor = pow(max(0.0, dot(normal, H)), u_shininess);                       \n"
	"	float diffactor = max(0.0, dot(normal, lDir));                        \n"
	"	color = texture(u_diffuseTex, v_texcoord) * diffactor * u_diffuseColor + specfactor * u_specularColor + u_ambientColor;    \n"
	"}                        \n"
};

class BaseShapeDrawApp : public Application{
public:

	virtual void Render(double elapseTime) override {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		_renderer.render();
	}

	virtual void Setup() override {

		//{
		//	auto sr = new ShapeRenderable(new SphereShape(50.0f), 32);
		//	auto material = new Material;
		//	auto program = new Program;
		//	program->attachShader(GL_VERTEX_SHADER, V_Shader);
		//	program->attachShader(GL_FRAGMENT_SHADER, F_Shader);
		//	material->setProgram(program);
		//	GLuint texID;
		//	TextureManager::Instance()->LoadTexture(texID, "datas/textures/land_shallow_topo_2048.jpg");
		//	material->setDiffuseTexture(texID);
		//	sr->setMaterial(material);
		//	_renderer.attachRenderableObject(sr);
		//}

		{
			auto sr = new ShapeRenderable(new BoxShape(20.0f, 20.0f, 20.0f), 1);
			auto material = new Material;
			auto program = new Program;
			program->attachShader(GL_VERTEX_SHADER, V_Shader);
			program->attachShader(GL_FRAGMENT_SHADER, F_Shader);
			material->setProgram(program);
			GLuint texID;
			TextureManager::Instance()->LoadTexture(texID, "datas/textures/land_shallow_topo_2048.jpg");
			material->setDiffuseTexture(texID);
			sr->setMaterial(material);
			_renderer.attachRenderableObject(sr);
		}

		//{
		//	auto sr = new ShapeRenderable(new CylinderShape(50.0f, 100.0f), 32);
		//	auto material = new Material;
		//	auto program = new Program;
		//	program->attachShader(GL_VERTEX_SHADER, V_Shader);
		//	program->attachShader(GL_FRAGMENT_SHADER, F_Shader);
		//	material->setProgram(program);
		//	GLuint texID;
		//	TextureManager::Instance()->LoadTexture(texID, "datas/textures/land_shallow_topo_2048.jpg");
		//	material->setDiffuseTexture(texID);
		//	sr->setMaterial(material);
		//	_renderer.attachRenderableObject(sr);
		//}


		//{
		//	auto sr = new ShapeRenderable(new CapsuleShape(50.0f, 50.0f), 32);
		//	auto material = new Material;
		//	auto program = new Program;
		//	program->attachShader(GL_VERTEX_SHADER, V_Shader);
		//	program->attachShader(GL_FRAGMENT_SHADER, F_Shader);
		//	material->setProgram(program);
		//	GLuint texID;
		//	TextureManager::Instance()->LoadTexture(texID, "datas/textures/land_shallow_topo_2048.jpg");
		//	material->setDiffuseTexture(texID);
		//	sr->setMaterial(material);
		//	_renderer.attachRenderableObject(sr);
		//}

		_renderer.setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		_renderer.depthTest(true);
	}

private:
};