#pragma once
#include <GL/glew.h>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Program.h"
#include "RenderableObject.h"

class Renderer;
class Mesh : public RenderableObject
{
public:
	
	Mesh(aiMesh *mesh, aiMaterial *material, const std::string &path);
	~Mesh();

	void setProgram(Program *program);
	virtual void draw(const Renderer *renderer) override;

private:

	void fillVertices();
	void fillIndices();
	void createShaderProgram();
	void initMaterial();
	void initDiffuse();

private:

	Program *_program;
	GLuint _VAO;
	aiMesh *_data;
	aiMaterial *_material;

	unsigned int _drawElementCount;
	GLenum _drawType;
	std::map<GLuint, std::pair<GLuint, GLuint> > _textureUnitMap;

	std::string _folderPath;
};

