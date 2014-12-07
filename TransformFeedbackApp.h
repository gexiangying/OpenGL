#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const GLchar *TF_Update_V_ShaderSource[] = {
	"#version 430                        \n"
	"layout (location = 0) in vec4 position_mass;                 \n"
	"layout (location = 1) in vec3 velocity;                      \n"
	"layout (location = 2) in ivec4 connection;                   \n"
	"layout (binding = 0) uniform samplerBuffer tex_position;     \n"
	"out vec4 tf_position_mass;                                   \n"
	"out vec3 tf_velocity;                                        \n"
	"uniform float t = 0.07;                                      \n"
	"uniform float k = 7.1;                                       \n"
	"const vec3 gravity = vec3(0.0, -0.08, 0.0);                  \n"
	"uniform float c = 2.8;                                       \n"
	"uniform float rest_length = 0.88;                            \n"
	"                                                             \n"
	"void main()                                                  \n"
	"{                                                            \n"
	"	vec3 p = position_mass.xyz;                                 \n"
	"	float m = position_mass.w;                                  \n"
	"	vec3 u = velocity;                                          \n"
	"	vec3 F = gravity * m - c * u;                               \n"
	"	bool fixed_node = true;                                     \n"
	"	for (int i = 0; i < 4; ++i)                                 \n"
	"	{                                                           \n"
	"		if (connection[i] != -1)                                  \n"
	"		{                                                         \n"
	"			vec3 q = texelFetch(tex_position, connection[i]).xyz;   \n"
	"			vec3 d = q - p;                                         \n"
	"			float x = length(d);                                    \n"
	"			F += -k * (rest_length - x) * normalize(d);             \n"
	"			fixed_node = false;                                     \n"
	"		}                                                         \n"
	"	}                                                           \n"
	"                                                             \n"
	"	if (fixed_node)                                             \n"
	"	{                                                           \n"
	"		F = vec3(0.0);                                            \n"
	"	}                                                           \n"
	"                                                             \n"
	"	vec3 a = F / m;                                             \n"
	"	vec3 s = u * t + 0.5 * a *t *t;                             \n"
	"	vec3 v = u + a * t;                                         \n"
	"	s = clamp(s, vec3(-25.0), vec3(25.0));                      \n"
	"	tf_position_mass = vec4(p + s, m);                          \n"
	"	tf_velocity = v;                                            \n"
	"}                                                            \n"
};

static const GLchar *TF_Render_V_ShaderSource[] = {
	"#version 430                                                    \n"
	"layout (location = 0) in vec4 position_mass;                 \n"
	"layout (location = 1) in vec3 velocity;                      \n"
	"layout (location = 2) in ivec4 connection;                   \n"
	"uniform mat4 mvp_Mat;                  \n"
	"void main()                                                     \n"
	"{                                                               \n"
	"	gl_Position = mvp_Mat * vec4(position_mass.xyz, 1.0);      \n"
	"}                                                               \n"
};

static const GLchar *TF_Render_F_ShaderSource[] = {
	"#version 430                        \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	color = vec4(1.0);                        \n"
	"}                        \n"
};

class TransformFeedbackApp : public Application{
public:

	enum BUFFER_TYPE_t
	{
		POSITION_A,
		POSITION_B,
		VELOCITY_A,
		VELOCITY_B,
		CONNECTION
	};

	enum
	{
		POINTS_X            = 50,
		POINTS_Y            = 50,
		POINTS_TOTAL        = (POINTS_X * POINTS_Y),
		CONNECTIONS_TOTAL   = (POINTS_X - 1) * POINTS_Y + (POINTS_Y - 1) * POINTS_X
	};

	TransformFeedbackApp()
		: GL_DOUBLE_BUFFER_SWAP(0)
	{}

	virtual void Render(double elapseTime) override {
		GLfloat color[] = {0.5f, 0.5f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(GL_Update_Program);
		glEnable(GL_RASTERIZER_DISCARD);
		for (int i = 0; i < 2; ++i){
			glBindVertexArray(GL_VAO[GL_DOUBLE_BUFFER_SWAP & 1]);
			glBindTexture(GL_TEXTURE_BUFFER, GL_TBO[GL_DOUBLE_BUFFER_SWAP & 1]);
			++GL_DOUBLE_BUFFER_SWAP;
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, GL_VBO[POSITION_A + (GL_DOUBLE_BUFFER_SWAP & 1)]);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, GL_VBO[VELOCITY_A + (GL_DOUBLE_BUFFER_SWAP & 1)]);

			glBeginTransformFeedback(GL_POINTS);
			glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
			glEndTransformFeedback();


		}
		glDisable(GL_RASTERIZER_DISCARD);

		glBindBuffer(GL_ARRAY_BUFFER, GL_VBO[POSITION_A]);
		glm::vec4 *buffer = (glm::vec4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glUseProgram(GL_Render_Program);
		GLint loc = glGetUniformLocation(GL_Render_Program, "mvp_Mat");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP_Matrix));
		glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
		glBindVertexArray(0);
	}

	virtual void Setup() override {
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 project = glm::perspective(30.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
		MVP_Matrix = project * view;

		glm::vec4 *initial_positions = new glm::vec4[POINTS_TOTAL];
		glm::vec3 *initial_velocities = new glm::vec3[POINTS_TOTAL];
		glm::ivec4 *connection_vectors = new glm::ivec4[POINTS_TOTAL];

		int n = 0;
		for (int j = 0; j < POINTS_Y; j++) {
			float fj = (float)j / (float)POINTS_Y;
			for (int i = 0; i < POINTS_X; i++) {
				float fi = (float)i / (float)POINTS_X;

				initial_positions[n] = glm::vec4((fi - 0.5f) * (float)POINTS_X,
					(fj - 0.5f) * (float)POINTS_Y,
					0.6f * sinf(fi) * cosf(fj),
					1.0f);
				initial_velocities[n] = glm::vec3(0.0f);

				connection_vectors[n] = glm::ivec4(-1);

				if (j != (POINTS_Y - 1))
				{
					if (i != 0)
						connection_vectors[n][0] = n - 1;

					if (j != 0)
						connection_vectors[n][1] = n - POINTS_X;

					if (i != (POINTS_X - 1))
						connection_vectors[n][2] = n + 1;

					if (j != (POINTS_Y - 1))
						connection_vectors[n][3] = n + POINTS_X;
				}
				n++;
			}
		}

		glGenVertexArrays(2, GL_VAO);
		glGenBuffers(5, GL_VBO);

		for (int i = 0; i < 2; ++i){
			glBindVertexArray(GL_VAO[i]);

			glBindBuffer(GL_ARRAY_BUFFER, GL_VBO[POSITION_A + i]);
			glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(glm::vec4), initial_positions, GL_DYNAMIC_COPY);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, GL_VBO[VELOCITY_A + i]);
			glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(glm::vec3), initial_velocities, GL_DYNAMIC_COPY);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, GL_VBO[CONNECTION]);
			glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(glm::ivec4), connection_vectors, GL_STATIC_DRAW);
			glVertexAttribIPointer(2, 4, GL_INT, 0, NULL);
			glEnableVertexAttribArray(2);
		}

		delete [] connection_vectors;
		delete [] initial_velocities;
		delete [] initial_positions;

		glGenTextures(2, GL_TBO);
		glBindTexture(GL_TEXTURE_BUFFER, GL_TBO[0]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, GL_VBO[POSITION_A]);
		glBindTexture(GL_TEXTURE_BUFFER, GL_TBO[1]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, GL_VBO[POSITION_B]);

		glPointSize(2);

		GL_Update_Program = CreateProgram(TF_Update_V_ShaderSource, NULL);
		static const char * tf_varyings[] = {
			"tf_position_mass",
			"tf_velocity"
		};
		glTransformFeedbackVaryings(GL_Update_Program, 2, tf_varyings, GL_SEPARATE_ATTRIBS);
		glLinkProgram(GL_Update_Program);

		GL_Render_Program = CreateProgram(TF_Render_V_ShaderSource, TF_Render_F_ShaderSource);
	}

private:

	GLuint CreateProgram(const GLchar * const *vShaderSource, const GLchar * const* fShaderSource){
		GLuint program = glCreateProgram();;

		bool state = true;
		if (vShaderSource){
			GLuint vShader= glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShader, 1, vShaderSource, NULL);
			if (!CompileShader(vShader)) return -1;
			glAttachShader(program, vShader);
			glDeleteShader(vShader);
		}

		if (fShaderSource){
			GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fShader, 1, fShaderSource, NULL);
			if (!CompileShader(fShader)) return -1;
			glAttachShader(program, fShader);
			glDeleteShader(fShader);
		}

		glLinkProgram(program);

		return program;
	}

private:

	GLuint GL_Update_Program;
	GLuint GL_Render_Program;
	GLuint GL_VAO[2];
	GLuint GL_VBO[5];
	GLuint GL_TBO[2];
	GLuint GL_DOUBLE_BUFFER_SWAP;
	glm::mat4 MVP_Matrix;
};