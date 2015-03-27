#include "AABoundingBox.h"


AABoundingBox::AABoundingBox(const glm::vec3 &min, const glm::vec3 &max)
	: _min(min)
	, _max(max)
{
}

AABoundingBox::AABoundingBox()
{

}


AABoundingBox::~AABoundingBox()
{
}
