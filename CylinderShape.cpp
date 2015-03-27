#include "CylinderShape.h"
#include "Geometry.h"
#include "Math.h"
#include "Array.h"


CylinderShape::CylinderShape(float radius, float height, const glm::mat4 &originMat)
	: Shape(ShapeType::CYLINDER_SHAPE, originMat)
	, _radius(radius)
	, _height(height)
{
}


CylinderShape::~CylinderShape()
{
}