#pragma once
#include "Shape.h"

class BoxShape : public Shape
{
public:
	BoxShape(float halfLength, float halfHeight, float halfWidth, const glm::mat4 &originMat = glm::mat4());
	~BoxShape();

	void setHalfLength(float halfLength) { _halfLength = halfLength; };
	float getHalfLength() const { return _halfLength; };
	void setHalfWidth(float halfWidth) { _halfWidth = halfWidth; };
	float getHalfWidth() const { return _halfWidth; };
	void setHalfHeight(float halfHeight) { _halfHeight = halfHeight; };
	float getHalfHeight() const { return _halfHeight; };

private:

	float _halfLength;
	float _halfWidth;
	float _halfHeight;
};

