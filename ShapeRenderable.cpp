#include "ShapeRenderable.h"
#include "Geometry.h"
#include "Shape.h"
#include "Math.h"
#include "SphereShape.h"
#include "BoxShape.h"
#include "CylinderShape.h"
#include "CapsuleShape.h"

ShapeRenderable::ShapeRenderable(Shape *shape, unsigned int segments)
	: _shape(shape)
	, _segments(segments)
	, _geometry(new Geometry)
{
	generateGeometry();
}


ShapeRenderable::~ShapeRenderable()
{
	delete _geometry;
}

void ShapeRenderable::draw( const Renderer *renderer, const glm::mat4 &modelMatrix )
{
	_geometry->draw(renderer, modelMatrix);
}

void ShapeRenderable::setMaterial( Material *material )
{
	_geometry->setMaterial(material);
}

Material* ShapeRenderable::getMaterial() const 
{
	return _geometry->getMaterial();
}

void ShapeRenderable::generateGeometry()
{
	switch (_shape->getShapeType())
	{
	case Shape::ShapeType::SPHERE_SHAPE:
		{
			buildSphere();
		}
		break;

	case Shape::ShapeType::BOX_SHAPE:
		{
			buildBox();
		}
		break;

	case Shape::ShapeType::CYLINDER_SHAPE:
		{
			buildCylinder();
		}
		break;

	case Shape::ShapeType::CAPSULE_SHAPE:
		{
			buildCapsule();
		}
		break;

	default:
		break;
	}
}

void ShapeRenderable::buildSphere()
{
	SphereShape *sphere = static_cast<SphereShape *>(_shape);

	unsigned int ring = _segments / 2;
	float deltaRing = MATH_PI / (float)(ring);
	float deltaSegment = 2.0f * MATH_PI / (float)_segments;
	float deltaV = 1.0f / (float)(ring);
	float deltaU = 1.0f / (float)_segments;

	Vec3Array vertexArray;
	Vec3Array normalArray;
	Vec4Array colorArray;
	Vec4Array texcoordArray;
	UIntArray indexArray;
	for(int j = 0; j < ring + 1; j++)  
	{  
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);

		for (int i = 0; i<_segments + 1; i++)  
		{  
			float x0 = radius * sinf(i * deltaSegment);  
			float z0 = radius * cosf(i * deltaSegment);  


			vertexArray.push_back(glm::vec3(x0, y0, z0) * sphere->getRadius());
			normalArray.push_back(glm::vec3(x0, y0, z0));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(i * deltaU, j * deltaV, 0.0f, 1.0f));
			if (j != ring) 
			{  
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}  
		}  
	} 

	Geometry::TextureCoordsArray tCoordsArray;
	tCoordsArray.push_back(texcoordArray);
	Geometry::IndicesArray indices;
	indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	_geometry->setVertexArray(vertexArray);
	_geometry->setNormalArray(normalArray);
	_geometry->setColorArray(colorArray);
	_geometry->setTextureCoordsArray(tCoordsArray);
	_geometry->setIndicesArray(indices);
}

void ShapeRenderable::buildBox()
{
	BoxShape *box = static_cast<BoxShape *>(_shape);
	float deltaLength = 2.0f * box->getHalfLength() / (float)_segments;
	float deltaWidth = 2.0f * box->getHalfWidth() / (float)_segments;
	float deltaHeight = 2.0f * box->getHalfHeight() / (float)_segments;
	float deltaV = 1.0f / (float)_segments;
	float deltaU = deltaV;

	Vec3Array vertexArray;
	Vec3Array normalArray;
	Vec4Array colorArray;
	Vec4Array texcoordArray;
	Geometry::IndicesArray indices;

	//front
	{
		UIntArray indexArray;
		for (int h = 0; h < _segments + 1; ++h){
			for (int l = 0; l < _segments + 1; ++l){
				float x0 = l * deltaLength;
				float y0 = h * deltaHeight;

				vertexArray.push_back(glm::vec3(x0, y0, 0.0f) + glm::vec3(-box->getHalfLength(), -box->getHalfHeight(), box->getHalfWidth()));
				normalArray.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
				colorArray.push_back(glm::vec4(1.0f));
				texcoordArray.push_back(glm::vec4(l * deltaU, h * deltaV, 0.0f, 1.0f));
				if (h != _segments){
					unsigned int vIndex = vertexArray.size() - 1;
					if (h % 2 == 0){
						indexArray.push_back(vIndex + _segments + 1);
						indexArray.push_back(vIndex);
					}else{
						indexArray.push_back(vIndex + _segments - 2.0f * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2.0f * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}


	//back
	{
		UIntArray indexArray;
		for (int h = 0; h < _segments + 1; ++h){
			for (int l = 0; l < _segments + 1; ++l){
				float x0 = -l * deltaLength;
				float y0 = h * deltaHeight;

				vertexArray.push_back(glm::vec3(x0, y0, 0.0f) + glm::vec3(box->getHalfLength(), -box->getHalfHeight(), -box->getHalfWidth()));
				normalArray.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
				colorArray.push_back(glm::vec4(1.0f));
				texcoordArray.push_back(glm::vec4(l * deltaU, h * deltaV, 0.0f, 1.0f));
				if (h != _segments){
					unsigned int vIndex = vertexArray.size() - 1;
					if (h % 2 == 0){
						indexArray.push_back(vIndex + _segments + 1);
						indexArray.push_back(vIndex);
					}else{
						indexArray.push_back(vIndex + _segments - 2.0f * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2.0f * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//left
	{
		UIntArray indexArray;
		for (int h = 0; h < _segments + 1; ++h){
			for (int w = 0; w < _segments + 1; ++w){
				float y0 = h * deltaHeight;
				float z0 = w * deltaWidth;

				vertexArray.push_back(glm::vec3(0.0f, y0, z0) + glm::vec3(-box->getHalfLength(), -box->getHalfHeight(), -box->getHalfWidth()));
				normalArray.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
				colorArray.push_back(glm::vec4(1.0f));
				texcoordArray.push_back(glm::vec4(w * deltaU, h * deltaV, 0.0f, 1.0f));
				if (h != _segments){
					unsigned int vIndex = vertexArray.size() - 1;
					if (h % 2 == 0){
						indexArray.push_back(vIndex + _segments + 1);
						indexArray.push_back(vIndex);
					}else{
						indexArray.push_back(vIndex + _segments - 2.0f * w);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2.0f * w);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//right
	{
		UIntArray indexArray;
		for (int h = 0; h < _segments + 1; ++h){
			for (int w = 0; w < _segments + 1; ++w){
				float y0 = h * deltaHeight;
				float z0 = -w * deltaWidth;

				vertexArray.push_back(glm::vec3(0.0f, y0, z0) + glm::vec3(box->getHalfLength(), -box->getHalfHeight(), box->getHalfWidth()));
				normalArray.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
				colorArray.push_back(glm::vec4(1.0f));
				texcoordArray.push_back(glm::vec4(w * deltaU, h * deltaV, 0.0f, 1.0f));
				if (h != _segments){
					unsigned int vIndex = vertexArray.size() - 1;
					if (h % 2 == 0){
						indexArray.push_back(vIndex + _segments + 1);
						indexArray.push_back(vIndex);
					}else{
						indexArray.push_back(vIndex + _segments - 2.0f * w);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2.0f * w);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//top
	{
		UIntArray indexArray;
		for (int w = 0; w < _segments + 1; ++w){
			for (int l = 0; l < _segments + 1; ++l){
				float x0 = l * deltaLength;
				float z0 = -w * deltaWidth;

				vertexArray.push_back(glm::vec3(x0, 0.0f, z0) + glm::vec3(-box->getHalfLength(), box->getHalfHeight(), box->getHalfWidth()));
				normalArray.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				colorArray.push_back(glm::vec4(1.0f));
				texcoordArray.push_back(glm::vec4(l * deltaU, w * deltaV, 0.0f, 1.0f));
				if (w != _segments){
					unsigned int vIndex = vertexArray.size() - 1;
					if (w % 2 == 0){
						indexArray.push_back(vIndex + _segments + 1);
						indexArray.push_back(vIndex);
					}else{
						indexArray.push_back(vIndex + _segments - 2.0f * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2.0f * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//bottom
	{
		UIntArray indexArray;
		for (int w = 0; w < _segments + 1; ++w){
			for (int l = 0; l < _segments + 1; ++l){
				float x0 = l * deltaLength;
				float z0 = w * deltaWidth;

				vertexArray.push_back(glm::vec3(x0, 0.0f, z0) + glm::vec3(-box->getHalfLength(), -box->getHalfHeight(), -box->getHalfWidth()));
				normalArray.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
				colorArray.push_back(glm::vec4(1.0f));
				texcoordArray.push_back(glm::vec4(l * deltaU, w * deltaV, 0.0f, 1.0f));
				if (w != _segments){
					unsigned int vIndex = vertexArray.size() - 1;
					if (w % 2 == 0){
						indexArray.push_back(vIndex + _segments + 1);
						indexArray.push_back(vIndex);
					}else{
						indexArray.push_back(vIndex + _segments - 2.0f * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2.0f * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	Geometry::TextureCoordsArray tCoordsArray;
	tCoordsArray.push_back(texcoordArray);
	_geometry->setVertexArray(vertexArray);
	_geometry->setNormalArray(normalArray);
	_geometry->setColorArray(colorArray);
	_geometry->setTextureCoordsArray(tCoordsArray);
	_geometry->setIndicesArray(indices);
}

void ShapeRenderable::buildCylinder()
{
	CylinderShape *cylinder = static_cast<CylinderShape *>(_shape);

	unsigned int rings = _segments / 2;
	float deltaRing = cylinder->getHeight() / (float)rings;
	float deltaSegment = 2.0f * MATH_PI / (float)_segments;
	float deltaV = 1.0f / (float)rings;
	float deltaU = 1.0f / (float)_segments;

	Vec3Array vertexArray;
	Vec3Array normalArray;
	Vec4Array colorArray;
	Vec4Array texcoordArray;
	UIntArray indexArray;

	//bottom circle
	for (int j = 0; j < 2; ++j)
	{
		for (int i = _segments; 0 <= i; i--)  
		{
			float x0 = j * sinf(i * deltaSegment);  
			float z0 = j * cosf(i * deltaSegment);  

			vertexArray.push_back(glm::vec3(x0 * cylinder->getRadius(), -cylinder->getHeight() / 2.0f, z0 * cylinder->getRadius()));
			normalArray.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(x0 * 0.5f + 0.5f, z0 * 0.5f + 0.5f, 0.0f, 1.0f));

			if (j != 1)
			{
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}
		}
	}


	for(int j = 0; j < rings + 1; j++)  
	{  
		float y0 = j * deltaRing - cylinder->getHeight() / 2.0f;

		for (int i = 0; i < _segments + 1; i++)  
		{  
			float x0 = sinf(i * deltaSegment);  
			float z0 = cosf(i * deltaSegment);  

			vertexArray.push_back(glm::vec3(x0 * cylinder->getRadius(), y0, z0 * cylinder->getRadius()));
			normalArray.push_back(glm::vec3(x0, 0.0f, z0));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(i * deltaU, j * deltaV, 0.0f, 1.0f));
			if (j != rings) 
			{  
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}  
		}  
	} 

	//top circle
	for (int j = 1; 0 <= j; j--)
	{
		for (int i = _segments; 0 <= i; i--)    
		{
			float x0 = j * sinf(i * deltaSegment);  
			float z0 = j * cosf(i * deltaSegment);  

			vertexArray.push_back(glm::vec3(x0 * cylinder->getRadius(), cylinder->getHeight() / 2.0f, z0 * cylinder->getRadius()));
			normalArray.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(x0 * 0.5f + 0.5f, z0 * 0.5f + 0.5f, 0.0f, 1.0f));

			if (j != 0)
			{
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}
		}
	}

	Geometry::TextureCoordsArray tCoordsArray;
	tCoordsArray.push_back(texcoordArray);
	Geometry::IndicesArray indices;
	indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	_geometry->setVertexArray(vertexArray);
	_geometry->setNormalArray(normalArray);
	_geometry->setColorArray(colorArray);
	_geometry->setTextureCoordsArray(tCoordsArray);
	_geometry->setIndicesArray(indices);
}

void ShapeRenderable::buildCapsule()
{
	CapsuleShape *capsule = static_cast<CapsuleShape*>(_shape);
	unsigned int rings = _segments / 2;
	float deltaRing = MATH_PI / (float)_segments;
	float deltaSegment = 2.0f * MATH_PI / (float)_segments;
	float deltaHeight = capsule->getHeight() / (float)rings;
	float deltaV = 1.0f / (float)rings;
	float deltaU = 1.0f / (float)_segments;

	Vec3Array vertexArray;
	Vec3Array normalArray;
	Vec4Array colorArray;
	Vec4Array texcoordArray;
	UIntArray indexArray;

	//bottom half sphere
	for (int j = 0; j < rings + 1; ++j)
	{
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);
		for (int i = 0; i< _segments + 1; i++)  
		{  
			float x0 = radius * sinf(i * deltaSegment);  
			float z0 = radius * cosf(i * deltaSegment);  

			vertexArray.push_back(glm::vec3(x0, y0, z0) * capsule->getRadius() - glm::vec3(0.0f, capsule->getHeight() / 2.0f, 0.0f));
			normalArray.push_back(glm::vec3(x0, y0, z0));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(i * deltaU, j * deltaU, 0.0f, 1.0f));
			if (j != (rings)) 
			{  
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}  
		}  
	}


	for(int j = 0; j < rings + 1; j++)  
	{  
		float y0 = j * deltaHeight - capsule->getHeight() / 2.0f;

		for (int i = 0; i < _segments + 1; i++)  
		{  
			float x0 = sinf(i * deltaSegment);  
			float z0 = cosf(i * deltaSegment);  

			vertexArray.push_back(glm::vec3(x0 * capsule->getRadius(), y0, z0 * capsule->getRadius()));
			normalArray.push_back(glm::vec3(x0, 0.0f, z0));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(i * deltaU, j * deltaV, 0.0f, 1.0f));
			if (j != rings) 
			{  
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}  
		}  
	} 

	//top half sphere
	for (int j = rings; j < _segments + 1; ++j)
	{
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);
		for (int i = 0; i< _segments + 1; i++)  
		{  
			float x0 = radius * sinf(i * deltaSegment);  
			float z0 = radius * cosf(i * deltaSegment);  

			vertexArray.push_back(glm::vec3(x0, y0, z0) * capsule->getRadius() + glm::vec3(0.0f, capsule->getHeight() / 2.0f, 0.0f));
			normalArray.push_back(glm::vec3(x0, y0, z0));
			colorArray.push_back(glm::vec4(1.0f));
			texcoordArray.push_back(glm::vec4(i * deltaU, j * deltaU, 0.0f, 1.0f));
			if (j != (_segments)) 
			{  
				unsigned int vIndex = vertexArray.size() - 1;
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}  
		}  
	}

	Geometry::TextureCoordsArray tCoordsArray;
	tCoordsArray.push_back(texcoordArray);
	Geometry::IndicesArray indices;
	indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	_geometry->setVertexArray(vertexArray);
	_geometry->setNormalArray(normalArray);
	_geometry->setColorArray(colorArray);
	_geometry->setTextureCoordsArray(tCoordsArray);
	_geometry->setIndicesArray(indices);
}
