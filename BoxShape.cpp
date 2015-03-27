#include "BoxShape.h"
#include "Geometry.h"
#include "Math.h"
#include "Array.h"


BoxShape::BoxShape(float halfLength, float halfWidth, float halfHeight, const glm::mat4 &originMat)
	: Shape(ShapeType::BOX_SHAPE, originMat)
	, _halfLength(halfLength)
	, _halfWidth(halfWidth)
	, _halfHeight(halfHeight)
{
}


BoxShape::~BoxShape()
{
}