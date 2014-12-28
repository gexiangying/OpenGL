#include "RenderQuad.h"
#include "Program.h"
#include "Renderer.h"
#include "Camera.h"
#include "TextureManager.h"
#include <sstream>

static const GLchar *RQ_V_ShaderSource[] = {
	"#version 430                        \n"
	"layout(location = 0) in vec4 position;                       \n"
	"layout(location = 1) in vec2 uv;                       \n"
	"uniform mat4 u_ModelViewProjectMat;                                      \n"
	"out vec2 v_uv;                                          \n"
	"void main(){                        \n"
	"	v_uv = uv;                                \n"
	"	gl_Position = u_ModelViewProjectMat * position;                        \n"
	"}                        \n"
};

static const GLchar *RQ_F_ShaderSource[] = {
	"#version 430                        \n"
	"uniform sampler2D u_Texture0;                       \n"
	"in vec2 v_uv;                                          \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	color = texture(u_Texture0, v_uv);     \n"
	"}                        \n"
};

RenderQuad::RenderQuad(float width, float height)
	: _inited(false)
{
	Vertex vertices[] = {
		{0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{width, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
		{0.0f,  height, 0.0f, 1.0f, 0.0f, 1.0f},
		{width,  height, 0.0f, 1.0f, 1.0f, 1.0f}
	};

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	GLuint buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_program = new Program;
	_program->attachShader(GL_VERTEX_SHADER, RQ_V_ShaderSource);
	_program->attachShader(GL_FRAGMENT_SHADER, RQ_F_ShaderSource);
	_program->link();
}


RenderQuad::~RenderQuad()
{
}

void RenderQuad::draw( const Renderer *renderer )
{
	init();
	auto camera = renderer->getCamera();
	if (!_program->isLinked())
		_program->link();
	_program->apply(camera->getProjectionMatrix(), camera->getViewMatrix());
	for (auto iter : _textures){
		glActiveTexture(GL_TEXTURE0 + iter.first);
		TextureManager::Instance()->BindTexture(iter.second.first);
		TextureManager::Instance()->BindSampler(iter.first, iter.second.second);
	}
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderQuad::setProgram( Program *program )
{
	_program = program;
	_inited = false;
}

void RenderQuad::setTexture( unsigned int texUnit, GLuint texID, GLuint sampID )
{
	_textures[texUnit].first = texID;
	_textures[texUnit].second = sampID;
	_inited = false;
}

void RenderQuad::init()
{
	if (!_inited){
		for (auto iter : _textures){
			std::stringstream ss;
			ss<<"u_Texture"<<iter.first;
			_program->setUniformi(ss.str(), iter.first);
		}
	}
	_inited = true;
}
