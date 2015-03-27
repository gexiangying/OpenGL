#pragma once
#include <glm/glm.hpp>

class BoundingSphere
{
public:
	BoundingSphere(const glm::vec3 &center, float radius);
	~BoundingSphere();

	void setCenter(const glm::vec3 &center) { _center = center; }
	const glm::vec3& getCenter() const { return _center; }
	void setRadius(float &radius) { _radius = radius; }
	float getRadius() const { return _radius; }

private:

	glm::vec3 _center;
	float _radius;
};

