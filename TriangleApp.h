#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "TextureManager.h"

struct  Vertex
{
	float x, y, z, w;
	float r, g, b, a;
	float u, v;
};

class TriangleApp : public Application{
public:

	virtual void Render(double elapseTime) override {
		GLfloat color[] = {0.5f, 0.5f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(GL_Program);
		GLint loc = glGetUniformLocation(GL_Program, "mvp_Mat");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP_Matrix));
		loc = glGetUniformLocation(GL_Program, "tex");
		glUniform1i(loc, 1);
		//GLfloat offset[] = {0.0f, 0.0f, 0.0f, 0.0f};
		//glVertexAttrib4fv(0, offset);


		glBindVertexArray(GL_VAO);
		//glActiveTexture(GL_TEXTURE0);
		//glActiveTexture(GL_TEXTURE1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	virtual void Setup() override {
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 project = glm::perspective(30.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
		MVP_Matrix = project * view;

		GL_Program = CreateProgram();
		glGenVertexArrays(1, &GL_VAO);
		glBindVertexArray(GL_VAO);
		GLuint buf;
		glGenBuffers(1, &buf);
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, 1024 * 1024, NULL, GL_STATIC_DRAW);
		static const Vertex vertices[] = {
			{-0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			{0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
			{-0.25f,  0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
			{0.25f,  0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f}
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, r));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		GLuint samps[2];
		glGenSamplers(2, samps);
		glSamplerParameteri(samps[0], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(samps[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(samps[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(samps[0], GL_TEXTURE_WRAP_T, GL_REPEAT);

		glSamplerParameteri(samps[1], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samps[1], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(samps[1], GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(samps[1], GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLuint textures[2];
		glGenTextures(2, textures);

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, samps[0]);
		//GLuint texid;
		//TextureManager::Instance()->LoadTexture(texid, "datas/sponza/maps/prozor1.jpg");
		//TextureManager::Instance()->BindTexture(texid);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 256, 256);
		float *data1 = GenTexture(256, 256, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_FLOAT, data1);

		glActiveTexture(GL_TEXTURE1);
		glBindSampler(1, samps[1]);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 256, 256);
		float *data2 = GenTexture(256, 256, 50);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_FLOAT, data2);

		//delete[] data1;
		//delete[] data2;
	}

protected:

	GLuint CreateProgram(){
		GLuint program = -1;
		static const GLchar *vShaderSource[] = {
			"#version 430                        \n"
			"layout(location = 0) in vec4 position;                       \n"
			"layout(location = 1) in vec4 color;                       \n"
			"layout(location = 2) in vec2 uv;                       \n"
			"uniform mat4 mvp_Mat;                                      \n"
			"out vec4 v_color;                                            \n"
			"out vec2 v_uv;                                          \n"
			"void main(){                        \n"
			//"	gl_Position = position[gl_VertexID];                        \n"
			"   v_color = color;                                \n"
			"	v_uv = uv;                                \n"
			"	gl_Position = mvp_Mat * position;                        \n"
			"}                        \n"
		};

		static const GLchar *fShaderSource[] = {
			"#version 430                        \n"
			"uniform sampler2D tex;                       \n"
			"in vec4 v_color;                                            \n"
			"in vec2 v_uv;                                          \n"
			"out vec4 color;                        \n"
			"void main(){                        \n"
			"	color = texture(tex, v_uv);                        \n"
			"}                        \n"
		};
		GLuint vShader= glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, vShaderSource, NULL);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, fShaderSource, NULL);
		if (CompileShader(vShader) && CompileShader(fShader)){
			program = glCreateProgram();
			glAttachShader(program, vShader);
			glAttachShader(program, fShader);
			glLinkProgram(program);
			glDeleteShader(vShader);
			glDeleteShader(fShader);
		}

		return program;
	}

	float* GenTexture(int width, int height, int offset){
		float *data = new float[width * height * 4];
		for (int h = 0; h < height; ++h){
			for (int w = 0; w < width; ++w){
				data[(h * width + w) * 4 + 0] = (float)((w & h) & 0xFF * offset) / 255.0f;
				data[(h * width + w) * 4 + 1] = (float)((w | h) & 0xFF * offset) / 255.0f;
				data[(h * width + w) * 4 + 2] = (float)((w ^ h) & 0xFF * offset) / 255.0f;
				data[(h * width + w) * 4 + 3] = 1.0f;
			}
		}

		return data;
	}

private:

	GLuint GL_Program;
	GLuint GL_VAO;
	glm::mat4 MVP_Matrix;
};