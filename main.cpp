#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stddef.h>

GLuint GL_Program;
GLuint GL_VAO;

bool CompileShader(GLuint shader){
	glCompileShader(shader);
	GLint state; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
	if (true){
		GLint maxLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
		if (maxLen > 0){
			GLchar *errors = new GLchar[maxLen];
			glGetShaderInfoLog(shader, maxLen, &maxLen, errors);
			printf("%s", errors);
			delete[] errors;
		}
	}
	return state == GL_TRUE? true: false;
}

GLuint CreateProgram(){
	GLuint program = -1;
	static const GLchar *vShaderSource[] = {
		"#version 430                        \n"
		"layout(location = 0) in vec4 position;                       \n"
		"layout(location = 1) in vec4 color;                       \n"
		"out vec4 v_color;                                            \n"
		"void main(){                        \n"
		//"	gl_Position = position[gl_VertexID];                        \n"
		"   v_color = color;                                \n"
		"	gl_Position = position;                        \n"
		"}                        \n"
	};

	static const GLchar *fShaderSource[] = {
		"#version 430                        \n"
		"in vec4 v_color;                                            \n"
		"out vec4 color;                        \n"
		"void main(){                        \n"
		"	color = v_color;                        \n"
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

void Render(){
	GLfloat color[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);

	glUseProgram(GL_Program);
	//GLfloat offset[] = {0.0f, 0.0f, 0.0f, 0.0f};
	//glVertexAttrib4fv(0, offset);

	glBindVertexArray(GL_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glFlush();
}

struct  Vertex
{
	float x, y, z, w;
	float r, g, b, a;
};

void Setup(){
	GL_Program = CreateProgram();
	glGenVertexArrays(1, &GL_VAO);
	glBindVertexArray(GL_VAO);
		GLuint buf;
		glGenBuffers(1, &buf);
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, 1024 * 1024, NULL, GL_STATIC_DRAW);
		static const Vertex vertices[] = {
			{0.25f, -0.25f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
			{-0.25f, -0.25f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
			{0.25f,  0.25f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, r));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glPointSize(40.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Test");
	glutDisplayFunc(Render);
	glewInit();
	Setup();
	glutMainLoop();
	return 0;
}