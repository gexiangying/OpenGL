#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

class Program
{
public:

	Program();
	~Program();

	void setUniformi(const GLchar *name, int value);
	void setUniformf(const GLchar *name, float value);
	void setUniformui(const GLchar *name, unsigned int value);
	void setUniform2f(const GLchar *name, const glm::vec2 &value);
	void setUniform3f(const GLchar *name, const glm::vec3 &value);
	void setUniform4f(const GLchar *name, const glm::vec4 &value);
	void setUniformMat2(const GLchar *name, const glm::mat2 &value);
	void setUniformMat3(const GLchar *name, const glm::mat3 &value);
	void setUniformMat4(const GLchar *name, const glm::mat4 &value);

	bool attachShader(GLenum type, const GLchar *sources[]);
	bool attachShader(GLenum type, const std::string &file);
	bool link();
	void apply(const glm::mat4 modelView);

protected:

	bool CompileShader( GLuint shader );
	void applyInnerUniforms(const glm::mat4 modelView);

private:

	GLuint _glProgram;
};

