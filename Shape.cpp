#include "Shape.h"


Shape::Shape(ShapeType type, const glm::mat4 &originMat)
	: _type(type)
	, _originMat(originMat)
{
}


Shape::~Shape()
{
}
