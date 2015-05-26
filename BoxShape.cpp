#include "BoxShape.h"
#include "Geometry.h"
#include "Math.h"
#include "Array.h"


BoxShape::BoxShape(float halfLength, float halfHeight, float halfWidth, const glm::mat4 &originMat)
	: Shape(ShapeType::BOX_SHAPE, originMat)
	, _halfLength(halfLength)
	, _halfHeight(halfHeight)
	, _halfWidth(halfWidth)
{
}


BoxShape::~BoxShape()
{
}