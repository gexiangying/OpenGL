#include "OBoundingBox.h"


OBoundingBox::OBoundingBox(const glm::vec3 &min, const glm::vec3 &max)
	: _min(min)
	, _max(max)
{
}


OBoundingBox::~OBoundingBox()
{
}
