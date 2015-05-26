#pragma once
#include <glm/glm.hpp>

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const glm::vec3 &min, const glm::vec3 &max);
	~BoundingBox();

	void setMin(const glm::vec3 &min) { _min = min; }
	const glm::vec3& getMin() const { return _min; }
	void setMax(const glm::vec3 &max) { _max = max; }
	const glm::vec3& getMax() const { return _max; }
	void expandBy(const glm::vec3 &point);
	void reset();

private:

	glm::vec3 _min;
	glm::vec3 _max;
};

