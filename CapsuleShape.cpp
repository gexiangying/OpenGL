#include "CapsuleShape.h"
#include "Geometry.h"
#include "Math.h"
#include "Array.h"


CapsuleShape::CapsuleShape(float radius, float height, const glm::mat4 &originMat)
	: Shape(ShapeType::CAPSULE_SHAPE, originMat)
	, _radius(radius)
	, _height(height)
{
}


CapsuleShape::~CapsuleShape()
{
}