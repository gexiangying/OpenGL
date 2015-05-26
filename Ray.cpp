#include "Ray.h"
#include "BoundingBox.h"

Ray::Ray(const glm::vec3 &origin, const glm::vec3 &dir)
	: _origin(origin)
	, _direction(dir)
{
}


Ray::~Ray()
{
}

bool Ray::intersectBoundingVolume( const BoundingBox &aabb )
{
	return false;
}

bool Ray::intersectBoundingVolume( const BoundingSphere &bs )
{
	return false;
}
