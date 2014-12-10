#include "Program.h"
#include "glm/gtc/type_ptr.hpp"
#include "Displayer.h"
#include <fstream>


Program::Program()
{
	_glProgram = glCreateProgram();
}


Program::~Program()
{
}

bool Program::attachShader( GLenum type, const GLchar *sources[] )
{
	GLuint shader= glCreateShader(type);
	glShaderSource(shader, sizeof(sources)/sizeof(*sources), sources, nullptr);
	if (!CompileShader(shader)) return false;

	glAttachShader(_glProgram, shader);

	return true;
}

bool Program::attachShader( GLenum type, const std::string &file )
{
	std::ifstream fin;  
	std::streamoff length;  
	fin.open(file.c_str(), std::ifstream::binary | std::ifstream::in);      // open input file  
	if (!fin.is_open()) return false;
	fin.seekg(0, std::ios::end);    // go to the end  
	length = fin.tellg();           // report location (this is the length)  
	fin.seekg(0, std::ios::beg);    // go back to the beginning  
	char *buffer = new char[length + 1];    // allocate memory for a buffer of appropriate dimension  
	fin.read(buffer, length);       // read the whole file into the buffer  
	buffer[length] = '\0';
	fin.close();                    // close file handle 

	GLuint shader= glCreateShader(type);
	glShaderSource(shader, 1, &buffer, nullptr);
	delete [] buffer;

	if (!CompileShader(shader)) return false;

	glAttachShader(_glProgram, shader);

	return true;
}

bool Program::link()
{
	glLinkProgram(_glProgram);
	glUseProgram(_glProgram);
	return true;
}

bool Program::CompileShader( GLuint shader )
{
	glCompileShader(shader);
	GLint state; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
	if (true){
		GLint maxLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
		if (maxLen > 0){
			GLchar *errors = new GLchar[maxLen];
			glGetShaderInfoLog(shader, maxLen, &maxLen, errors);

			if (strcmp(errors, "") != 0){
				printf("\n--------CompileShader Errors--------\n");
				printf("%s", errors);
				printf("\n--------CompileShader Errors--------\n");
			}
			delete[] errors;
		}
	}
	return state == GL_TRUE? true: false;
}

void Program::apply(const glm::mat4 modelView)
{
	glUseProgram(_glProgram);
	applyInnerUniforms(modelView);
}

void Program::setUniformi( const GLchar *name, int value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniform1i(loc, value);
}

void Program::setUniformf( const GLchar *name, float value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniform1f(loc, value);
}

void Program::setUniformui( const GLchar *name, unsigned int value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniform1ui(loc, value);
}

void Program::setUniform2f( const GLchar *name, const glm::vec2 &value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniform2f(loc, value.x, value.y);
}

void Program::setUniform3f( const GLchar *name, const glm::vec3 &value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniform3f(loc, value.x, value.y, value.z);
}

void Program::setUniform4f( const GLchar *name, const glm::vec4 &value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Program::setUniformMat2( const GLchar *name, const glm::mat2 &value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::setUniformMat3( const GLchar *name, const glm::mat3 &value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::setUniformMat4( const GLchar *name, const glm::mat4 &value )
{
	GLint loc = glGetUniformLocation(_glProgram, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::applyInnerUniforms(const glm::mat4 modelView)
{
	GLint loc = glGetUniformLocation(_glProgram, "u_ModelViewProjectMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(Displayer::Instance()->getProjectionMatrix() * modelView));

	loc = glGetUniformLocation(_glProgram, "u_ModelViewMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelView));

	loc = glGetUniformLocation(_glProgram, "u_NormalMat");
	glm::mat3 normalMat(modelView[0].x, modelView[0].y, modelView[0].z
		, modelView[1].x, modelView[1].y, modelView[1].z
		, modelView[2].x, modelView[2].y, modelView[2].z);
	normalMat = glm::inverse(normalMat);
	normalMat = glm::transpose(normalMat);
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(normalMat));
}
