#pragma once
#include <glm/glm.hpp>

class Geometry;
class Shape
{
public:

	enum class ShapeType
	{
		SPHERE_SHAPE,
		BOX_SHAPE,
		CYLINDER_SHAPE,
		CAPSULE_SHAPE,
	};

	ShapeType getShapeType() const { return _type; }
	void setOriginMat(const glm::mat4 &originMat) { _originMat = originMat; };
	const glm::mat4& getOriginMat() const { return _originMat; };

protected:

	Shape(ShapeType type, const glm::mat4 &originMat);
	~Shape();

protected:

	ShapeType _type;
	glm::mat4 _originMat;
};

