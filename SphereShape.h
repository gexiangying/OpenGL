#pragma once
#include "Shape.h"

class SphereShape : public Shape
{
public:
	SphereShape(float radius, const glm::mat4 &originMat = glm::mat4());
	~SphereShape();

	void setRadius(float radius) { _radius = radius; };
	float getRadius() const { return _radius; };

private:

	float _radius;
};

