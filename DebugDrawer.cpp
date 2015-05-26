#include "DebugDrawer.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Math.h"
#include "Renderer.h"
#include "Program.h"
#include "Material.h"

static const GLchar *Material_V_Shader[] = {
	"#version 430                                                    \n"
	"layout (location = 0) in vec3 position;                        \n"
	"layout (location = 1) in vec3 normal;                          \n"
	"layout (location = 2) in vec4 texcoord;                         \n"
	"layout (location = 3) in vec4 color;                         \n"
	"uniform mat4 u_ModelViewProjectMat;                              \n"
	"out vec4 v_color;                                             \n"
	"void main()                                                     \n"
	"{                                                               \n"
	"	v_color = color;                             \n"
	"	gl_Position = u_ModelViewProjectMat * vec4(position, 1.0);                 \n"
	"}                                                               \n"
};

static const GLchar *Material_F_Shader[] = {
	"#version 430                        \n"
	"in vec4 v_color;                                             \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	color = v_color;    \n"
	"}                        \n"
};

DebugDrawer::DebugDrawer(Renderer *renderer)
	: _geometry(new Geometry)
	, _renderer(renderer)
{
	_renderer->attachRenderableObject(_geometry);
	auto program = new Program;
	program->attachShader(GL_VERTEX_SHADER, Material_V_Shader);
	program->attachShader(GL_FRAGMENT_SHADER, Material_F_Shader);
	program->link();
	auto material = new Material;
	material->setProgram(program);
	_geometry->setMaterial(material);
}


DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::drawBoundingBox( const BoundingBox *bb, const glm::vec4 &color )
{
	drawLine(glm::vec3(bb->getMin().x, bb->getMin().y, bb->getMin().z), glm::vec3(bb->getMax().x, bb->getMin().y, bb->getMin().z), color);
	drawLine(glm::vec3(bb->getMax().x, bb->getMin().y, bb->getMin().z), glm::vec3(bb->getMax().x, bb->getMax().y, bb->getMin().z), color);
	drawLine(glm::vec3(bb->getMax().x, bb->getMax().y, bb->getMin().z), glm::vec3(bb->getMin().x, bb->getMax().y, bb->getMin().z), color);
	drawLine(glm::vec3(bb->getMin().x, bb->getMax().y, bb->getMin().z), glm::vec3(bb->getMin().x, bb->getMin().y, bb->getMin().z), color);

	drawLine(glm::vec3(bb->getMin().x, bb->getMin().y, bb->getMax().z), glm::vec3(bb->getMax().x, bb->getMin().y, bb->getMax().z), color);
	drawLine(glm::vec3(bb->getMax().x, bb->getMin().y, bb->getMax().z), glm::vec3(bb->getMax().x, bb->getMax().y, bb->getMax().z), color);
	drawLine(glm::vec3(bb->getMax().x, bb->getMax().y, bb->getMax().z), glm::vec3(bb->getMin().x, bb->getMax().y, bb->getMax().z), color);
	drawLine(glm::vec3(bb->getMin().x, bb->getMax().y, bb->getMax().z), glm::vec3(bb->getMin().x, bb->getMin().y, bb->getMax().z), color);

	drawLine(glm::vec3(bb->getMin().x, bb->getMin().y, bb->getMin().z), glm::vec3(bb->getMin().x, bb->getMin().y, bb->getMax().z), color);
	drawLine(glm::vec3(bb->getMax().x, bb->getMin().y, bb->getMin().z), glm::vec3(bb->getMax().x, bb->getMin().y, bb->getMax().z), color);
	drawLine(glm::vec3(bb->getMax().x, bb->getMax().y, bb->getMin().z), glm::vec3(bb->getMax().x, bb->getMax().y, bb->getMax().z), color);
	drawLine(glm::vec3(bb->getMin().x, bb->getMax().y, bb->getMin().z), glm::vec3(bb->getMin().x, bb->getMax().y, bb->getMax().z), color);
}

void DebugDrawer::drawBoundingSphere( const BoundingSphere *bs, const glm::vec4 &color )
{
	unsigned int ring = 5;
	unsigned int segments = 10;
	float deltaRing = MATH_PI / (float)(ring);
	float deltaSegment = 2.0f * MATH_PI / (float)segments;

	for(unsigned int j = 0; j < ring; j++)  
	{  
		float radius0 = sinf(j * deltaRing);
		float radius1 = sinf((j+1) * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);
		float y1 = cosf(MATH_PI - (j+1) * deltaRing);

		for (unsigned int i = 0; i < segments; i++)  
		{  
			float x0 = radius0 * sinf(i * deltaSegment);  
			float z0 = radius0 * cosf(i * deltaSegment);  
			float x1 = radius1 * sinf((i+1) * deltaSegment);  
			float z1 = radius1 * cosf((i+1) * deltaSegment);  

			drawLine(glm::vec3(x0, y0, z0), glm::vec3(x1, y0, z1), color);
			drawLine(glm::vec3(x0, y0, z0), glm::vec3(x0, y1, z0), color);
		}  
	} 
}

void DebugDrawer::drawLine( const glm::vec3 &from, const glm::vec3 &to, const glm::vec4 &color )
{
	ensureBuffer(2);

	glm::vec3* vertices = (glm::vec3*)_geometry->mapArray(Geometry::MapType::MT_VERTEX);
	vertices[_drawVertexCount] = from;
	vertices[_drawVertexCount + 1] = to;
	_geometry->unMapArray(Geometry::MapType::MT_VERTEX);

	glm::vec4* colors = (glm::vec4*)_geometry->mapArray(Geometry::MapType::MT_COLOR);
	colors[_drawVertexCount] = color;
	colors[_drawVertexCount + 1] = color;
	_geometry->unMapArray(Geometry::MapType::MT_COLOR);

	_drawVertexCount += 2;
	_geometry->setDrawArrays(Geometry::GT_LINES, 0, _drawVertexCount);
}

void DebugDrawer::dirtyDraw()
{
	_drawVertexCount = 0;
}

void DebugDrawer::ensureBuffer( unsigned int count )
{
	if (_vertices.size() < (_drawVertexCount + count)){
		unsigned int sz = glm::max(_drawVertexCount, count);
		_vertices.resize(_vertices.size() + 2 * sz);
		_colors.resize(_vertices.size() + 2 * sz);
		_geometry->setVertexArray(_vertices, Geometry::DS_STREAM);
		_geometry->setColorArray(_colors, Geometry::DS_STREAM);
	}
}
