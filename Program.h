#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <functional>

class Program
{
public:

	Program();
	~Program();

	void setName(const std::string &name) { _name = name; }
	const std::string &getName()const { return _name; }
	void setUniformi(const std::string &name, int value);
	void setUniformf(const std::string &name, float value);
	void setUniformui(const std::string &name, unsigned int value);
	void setUniform2f(const std::string &name, const glm::vec2 &value);
	void setUniform3f(const std::string &name, const glm::vec3 &value);
	void setUniform4f(const std::string &name, const glm::vec4 &value);
	void setUniformMat2(const std::string &name, const glm::mat2 &value);
	void setUniformMat3(const std::string &name, const glm::mat3 &value);
	void setUniformMat4(const std::string &name, const glm::mat4 &value);
	void setUniformiv(const std::string &name, int *values, unsigned int count);
	void setUniformfv(const std::string &name, float *values, unsigned int count);
	void setUniformuiv(const std::string &name, unsigned int *values, unsigned int count);
	void setUniform2fv(const std::string &name, const glm::vec2 *values, unsigned int count);
	void setUniform3fv(const std::string &name, const glm::vec3 *values, unsigned int count);
	void setUniform4fv(const std::string &name, const glm::vec4 *values, unsigned int count);
	void setUniformMat2v(const std::string &name, const glm::mat2 *values, unsigned int count);
	void setUniformMat3v(const std::string &name, const glm::mat3 *values, unsigned int count);
	void setUniformMat4v(const std::string &name, const glm::mat4 *values, unsigned int count);

	bool attachShader(GLenum type, const GLchar *sources[]);
	bool attachShader(GLenum type, const std::string &file);
	bool link();
	bool isLinked() { return _isLinked; };
	void apply(const glm::mat4 &projection, const glm::mat4 &modelView);

protected:

	bool CompileShader( GLuint shader, const std::string &type );
	void applyUniforms();
	void applyInnerUniforms(const glm::mat4 &projection, const glm::mat4 &modelView);
	std::string getShaderType(GLenum glType);

private:

	GLuint _glProgram;
	std::string _name;
	bool _isLinked;
	bool _isUsed;

	std::vector<std::function<void()> > _uniforms;
};

