#pragma once
#include <GL/glew.h>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Mesh
{
public:
	
	Mesh(aiMesh *mesh, aiMaterial *material);
	~Mesh();

	void draw(const glm::mat4x4 &modelView);

private:

	void fillVertices();
	void fillIndices();
	void createShaderProgram();
	bool CompileShader(GLuint shader);

private:

	GLuint _program;
	GLuint _VAO;
	aiMesh *_data;
	aiMaterial *_material;

	unsigned int _drawElementCount;
	GLenum _drawType;
};

