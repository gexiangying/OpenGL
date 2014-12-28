#include "Program.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>


Program::Program()
	: _isLinked(false)
	, _isUsed(false)
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
	if (!CompileShader(shader, getShaderType(type))) return false;

	glAttachShader(_glProgram, shader);
	_isLinked = false;
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

	if (!CompileShader(shader, getShaderType(type))) return false;

	glAttachShader(_glProgram, shader);
	_isLinked = false;
	return true;
}

bool Program::link()
{
	if (!_isLinked)
		glLinkProgram(_glProgram);
	_isLinked = true;
	return true;
}

bool Program::CompileShader( GLuint shader, const std::string &type )
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
				printf("\n--------%s Shader Errors--------\n", type.c_str());
				printf("%s", errors);
			}
			delete[] errors;
		}
	}
	return state == GL_TRUE? true: false;
}

void Program::apply(const glm::mat4 &projection, const glm::mat4 &modelView)
{
	glUseProgram(_glProgram);
	applyInnerUniforms(projection, modelView);
	applyUniforms();
}

void Program::setUniformi( const std::string &name, int value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform1i(loc, value);
	});
}

void Program::setUniformf( const std::string &name, float value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform1f(loc, value);
	});
}

void Program::setUniformui( const std::string &name, unsigned int value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform1ui(loc, value);
	});
}

void Program::setUniform2f( const std::string &name, const glm::vec2 &value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform2f(loc, value.x, value.y);
	});
}

void Program::setUniform3f( const std::string &name, const glm::vec3 &value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform3f(loc, value.x, value.y, value.z);
	});
}

void Program::setUniform4f( const std::string &name, const glm::vec4 &value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform4f(loc, value.x, value.y, value.z, value.w);
	});
}

void Program::setUniformMat2( const std::string &name, const glm::mat2 &value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	});
}

void Program::setUniformMat3( const std::string &name, const glm::mat3 &value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	});
}

void Program::setUniformMat4( const std::string &name, const glm::mat4 &value )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	});
}

void Program::setUniformiv( const std::string &name, int *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform1iv(loc, count, values);
	});
}

void Program::setUniformfv( const std::string &name, float *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform1fv(loc, count, values);
	});
}

void Program::setUniformuiv( const std::string &name, unsigned int *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform1uiv(loc, count, values);
	});
}

void Program::setUniform2fv( const std::string &name, const glm::vec2 *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform2fv(loc, count, glm::value_ptr(*values));
	});
}

void Program::setUniform3fv( const std::string &name, const glm::vec3 *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform3fv(loc, count, glm::value_ptr(*values));
	});
}

void Program::setUniform4fv( const std::string &name, const glm::vec4 *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniform4fv(loc, count, glm::value_ptr(*values));
	});
}

void Program::setUniformMat2v( const std::string &name, const glm::mat2 *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniformMatrix2fv(loc, count, GL_FALSE, glm::value_ptr(*values));
	});
}

void Program::setUniformMat3v( const std::string &name, const glm::mat3 *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniformMatrix3fv(loc, count, GL_FALSE, glm::value_ptr(*values));
	});
}

void Program::setUniformMat4v( const std::string &name, const glm::mat4 *values, unsigned int count )
{
	_uniforms.push_back([=]{
		GLint loc = glGetUniformLocation(_glProgram, name.c_str());
		glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(*values));
	});
}

void Program::applyInnerUniforms(const glm::mat4 &projection, const glm::mat4 &modelView)
{
	GLint loc = glGetUniformLocation(_glProgram, "u_ModelViewProjectMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection * modelView));

	loc = glGetUniformLocation(_glProgram, "u_ModelViewMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelView));

	loc = glGetUniformLocation(_glProgram, "u_ProjectMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

	loc = glGetUniformLocation(_glProgram, "u_NormalMat");
	glm::mat3 normalMat(modelView[0].x, modelView[0].y, modelView[0].z
		, modelView[1].x, modelView[1].y, modelView[1].z
		, modelView[2].x, modelView[2].y, modelView[2].z);
	normalMat = glm::inverse(normalMat);
	normalMat = glm::transpose(normalMat);
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(normalMat));
}

void Program::applyUniforms()
{
	for (auto iter : _uniforms){
		iter();
	}
}

std::string Program::getShaderType( GLenum glType )
{
	switch (glType)
	{
	case GL_VERTEX_SHADER:
		return "GL_VERTEX_SHADER";

	case GL_TESS_CONTROL_SHADER:
		return "GL_TESS_CONTROL_SHADER";

	case GL_TESS_EVALUATION_SHADER:
		return "GL_TESS_EVALUATION_SHADER";

	case GL_GEOMETRY_SHADER:
		return "GL_GEOMETRY_SHADER";

	case GL_FRAGMENT_SHADER:
		return "GL_FRAGMENT_SHADER";

	case GL_COMPUTE_SHADER:
		return "GL_COMPUTE_SHADER";

	default:
		break;
	}

	return "UnKnow";
}
