#pragma once
#include <glm/glm.hpp>

class AABoundingBox;
class OBoundingBox;
class BoundingSphere;
class Ray
{
public:
	Ray(const glm::vec3 &origin, const glm::vec3 &dir);
	~Ray();

	void setOrigin(const glm::vec3 &origin) { _origin = origin; }
	const glm::vec3& getOrigin() const { return _origin; }
	void setDirection(const glm::vec3 &dir) { _direction = dir; }
	const glm::vec3& getDirection() const { return _direction; }

	bool intersectBoundingVolume(const AABoundingBox &aabb);
	bool intersectBoundingVolume(const OBoundingBox &obb);
	bool intersectBoundingVolume(const BoundingSphere &bs);

private:

	glm::vec3 _origin;
	glm::vec3 _direction;
};

