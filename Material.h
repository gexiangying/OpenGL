#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Program;
class Material
{
public:
	Material();
	~Material();

	void apply(const glm::mat4 &projection, const glm::mat4 &modelView);

	void setProgram(Program *program) { _program = program; };
	Program* getProgram() const {return _program; }
	void setDiffuseColor(const glm::vec4 &diffuseColor){ _diffuseColor = diffuseColor; };
	const glm::vec4& getDiffuseColor() const { return _diffuseColor; }

	void setSpecularColor(const glm::vec4 &specularColor){ _specularColor = specularColor; };
	const glm::vec4& getSpecularColor() const { return _specularColor; }

	void setAmbientColor(const glm::vec4 &amibientColor){ _amibientColor = amibientColor; };
	const glm::vec4& getAmbientColor() const { return _amibientColor; }

	void setEmissionColor(const glm::vec4 &emissionColor){ _emissionColor = emissionColor; };
	const glm::vec4& getEmissionColor() const { return _emissionColor; }

	void setShininess(float shininess){ _shininess = shininess; };
	float getShininess() const { return _shininess; }

	void setDiffuseTexture(GLuint texID){ _diffuseTexture = texID; };
	GLuint getDiffuseTexture() const { return _diffuseTexture; }

	void setSpecularTexture(GLuint texID){ _specularTexture = texID; };
	GLuint getSpecularTexture() const { return _specularTexture; }

	void setAmbientTexture(GLuint texID){ _ambientTexture = texID; };
	GLuint getAmbientTexture() const { return _ambientTexture; }

	void setDiffuseTextureSampler(GLuint sampID){ _diffuseTexSampler = sampID; };
	GLuint getDiffuseTextureSampler() const { return _diffuseTexSampler; }

	void setSpecularTextureSampler(GLuint sampID){ _specularTexSampler = sampID; };
	GLuint getSpecularTextureSampler() const { return _specularTexSampler; }

	void setAmbientTextureSampler(GLuint sampID){ _ambientTexSampler = sampID; };
	GLuint getAmbientTextureSampler() const { return _ambientTexSampler; }

private:

	Program *_program;
	glm::vec4 _diffuseColor;
	glm::vec4 _specularColor;
	glm::vec4 _amibientColor;
	glm::vec4 _emissionColor;
	float _shininess;

	GLuint _diffuseTexture;
	GLuint _specularTexture;
	GLuint _ambientTexture;
	GLuint _diffuseTexSampler;
	GLuint _specularTexSampler;
	GLuint _ambientTexSampler;
};

