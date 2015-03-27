#pragma once
#include "RenderableObject.h"

class Geometry;
class Shape;
class ShapeRenderable : public RenderableObject
{
public:
	ShapeRenderable(Shape *shape, unsigned int segments);
	virtual ~ShapeRenderable();

	virtual void draw(const Renderer *renderer, const glm::mat4 &modelMatrix) override;
	virtual void setMaterial(Material *material) override;
	virtual Material* getMaterial() const override;

protected:

	void generateGeometry();
	void buildSphere();
	void buildBox();
	void buildCylinder();
	void buildCapsule();

protected:

	unsigned int _segments;
	Shape *_shape;
	Geometry *_geometry;
};

