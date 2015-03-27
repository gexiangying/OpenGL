#include "SphereShape.h"
#include "Geometry.h"
#include "Math.h"
#include "Array.h"


SphereShape::SphereShape(float radius, const glm::mat4 &originMat)
	: Shape(ShapeType::SPHERE_SHAPE, originMat)
	, _radius(radius)
{
}


SphereShape::~SphereShape()
{
}