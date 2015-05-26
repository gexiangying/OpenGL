#pragma once
#include <glm/glm.hpp>
#include "Geometry.h"

class BoundingBox;
class BoundingSphere;
class DebugDrawer
{
public:
	DebugDrawer(Renderer *renderer);
	~DebugDrawer();

	void drawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec4 &color);
	void drawBoundingBox(const BoundingBox *bb, const glm::vec4 &color);
	void drawBoundingSphere(const BoundingSphere *bs, const glm::vec4 &color);

	void dirtyDraw();

protected:

	void init();
	void ensureBuffer(unsigned int count);

private:

	Renderer *_renderer;
	Geometry *_geometry;
	Vec3Array _vertices;
	Vec4Array _colors;

	unsigned int _drawVertexCount;
};

