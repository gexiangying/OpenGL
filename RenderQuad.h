#pragma once
#include "RenderableObject.h"
#include <GL/glew.h>
#include <map>

class Program;
class RenderQuad : public RenderableObject
{
public:
	RenderQuad(float width, float height);
	~RenderQuad();

	virtual void draw(const Renderer *renderer, const glm::mat4 &modelMatrix) override;
	void setProgram(Program *program);
	void setTexture(unsigned int texUnit, GLuint texID, GLuint sampID);

private:

	void init();
	
private:

	struct  Vertex
	{
		float x, y, z, w;
		float u, v;
	};
	Program *_program;
	GLuint _vao;
	std::map<unsigned int, std::pair<GLuint, GLuint> > _textures;
	bool _inited;
};

