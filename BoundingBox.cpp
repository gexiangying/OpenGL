#include "BoundingBox.h"


BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max)
	: _min(min)
	, _max(max)
{
}

BoundingBox::BoundingBox()
	: _min(FLT_MAX)
	, _max(-FLT_MAX)
{

}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::expandBy( const glm::vec3 &point )
{
	_min.x = glm::min(_min.x, point.x);
	_min.y = glm::min(_min.y, point.y);
	_min.z = glm::min(_min.z, point.z);
	_max.x = glm::max(_max.x, point.x);
	_max.y = glm::max(_max.y, point.y);
	_max.z = glm::max(_max.z, point.z);
}

void BoundingBox::reset()
{
	_min = glm::vec3(FLT_MAX);
	_max = glm::vec3(-FLT_MAX);
}
