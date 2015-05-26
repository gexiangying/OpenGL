#pragma once
#include <glm/glm.hpp>
class Renderer;
class Material;
class BoundingSphere;
class BoundingBox;
class RenderableObject
{
public:
	RenderableObject();
	virtual ~RenderableObject();

	virtual void draw(const Renderer *renderer, const glm::mat4 &modelMatrix) = 0;
	virtual BoundingSphere* getBoundingSphere() { return nullptr; };
	virtual BoundingBox* getBoundingBox() { return nullptr; };
	virtual void setMaterial(Material *material) {};
	virtual Material* getMaterial() const { return nullptr; };
};

