#pragma once
#include "Shape.h"

class CylinderShape : public Shape
{
public:
	CylinderShape(float radius, float height, const glm::mat4 &originMat = glm::mat4());
	~CylinderShape();

	void setRadius(float radius) { _radius = radius; };
	float getRadius() const { return _radius; };
	void setHeight(float height) { _height = height; };
	float getHeight() const { return _height; };

private:

	float _radius;
	float _height;
};

