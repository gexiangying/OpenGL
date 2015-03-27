#include "BoundingSphere.h"


BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius)
	: _center(center)
	, _radius(radius)
{
}


BoundingSphere::~BoundingSphere()
{
}
