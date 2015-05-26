#include "ShapeRenderable.h"
#include "Geometry.h"
#include "Shape.h"
#include "Math.h"
#include "SphereShape.h"
#include "BoxShape.h"
#include "CylinderShape.h"
#include "CapsuleShape.h"

ShapeRenderable::ShapeRenderable(Shape *shape, ShapeRenderType srt, unsigned int segments)
	: _shape(shape)
	, _renderType(srt)
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
			if (_renderType == ShapeRenderType::SR_FILL)
				buildSphere();
			else
				buildSphereLines();
		}
		break;

	case Shape::ShapeType::BOX_SHAPE:
		{
			if (_renderType == ShapeRenderType::SR_FILL)
				buildBox();
			else
				buildBoxLines();
		}
		break;

	case Shape::ShapeType::CYLINDER_SHAPE:
		{
			if (_renderType == ShapeRenderType::SR_FILL)
				buildCylinder();
			else
				buildCylinderLines();
		}
		break;

	case Shape::ShapeType::CAPSULE_SHAPE:
		{
			if (_renderType == ShapeRenderType::SR_FILL)
				buildCapsule();
			else
				buildCapsuleLines();
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
	for(unsigned int j = 0; j < ring + 1; j++)  
	{  
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);

		for (unsigned int i = 0; i<_segments + 1; i++)  
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
		for (unsigned int h = 0; h < _segments + 1; ++h){
			for (unsigned int l = 0; l < _segments + 1; ++l){
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
						indexArray.push_back(vIndex + _segments - 2 * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2 * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}


	//back
	{
		UIntArray indexArray;
		for (unsigned int h = 0; h < _segments + 1; ++h){
			for (unsigned int l = 0; l < _segments + 1; ++l){
				float x0 = -(l * deltaLength);
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
						indexArray.push_back(vIndex + _segments - 2 * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2 * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//left
	{
		UIntArray indexArray;
		for (unsigned int h = 0; h < _segments + 1; ++h){
			for (unsigned int w = 0; w < _segments + 1; ++w){
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
						indexArray.push_back(vIndex + _segments - 2 * w);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2 * w);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//right
	{
		UIntArray indexArray;
		for (unsigned int h = 0; h < _segments + 1; ++h){
			for (unsigned int w = 0; w < _segments + 1; ++w){
				float y0 = h * deltaHeight;
				float z0 = -(w * deltaWidth);

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
						indexArray.push_back(vIndex + _segments - 2 * w);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2 * w);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//top
	{
		UIntArray indexArray;
		for (unsigned int w = 0; w < _segments + 1; ++w){
			for (unsigned int l = 0; l < _segments + 1; ++l){
				float x0 = l * deltaLength;
				float z0 = -(w * deltaWidth);

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
						indexArray.push_back(vIndex + _segments - 2 * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2 * l);
					}
				}
			}
		}

		indices.push_back(std::make_pair(Geometry::GT_TRIANGLE_STRIP, indexArray));
	}

	//bottom
	{
		UIntArray indexArray;
		for (unsigned int w = 0; w < _segments + 1; ++w){
			for (unsigned int l = 0; l < _segments + 1; ++l){
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
						indexArray.push_back(vIndex + _segments - 2 * l);
						indexArray.push_back(vIndex + _segments + 1 + _segments - 2 * l);
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
	for (unsigned int j = 0; j < 2; ++j)
	{
		for (unsigned int i = _segments; 0 <= i; i--)  
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


	for(unsigned int j = 0; j < rings + 1; j++)  
	{  
		float y0 = j * deltaRing - cylinder->getHeight() / 2.0f;

		for (unsigned int i = 0; i < _segments + 1; i++)  
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
	for (unsigned int j = 1; 0 <= j; j--)
	{
		for (unsigned int i = _segments; 0 <= i; i--)    
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
	for (unsigned int j = 0; j < rings + 1; ++j)
	{
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);
		for (unsigned int i = 0; i< _segments + 1; i++)  
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


	for(unsigned int j = 0; j < rings + 1; j++)  
	{  
		float y0 = j * deltaHeight - capsule->getHeight() / 2.0f;

		for (unsigned int i = 0; i < _segments + 1; i++)  
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
	for (unsigned int j = rings; j < _segments + 1; ++j)
	{
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);
		for (unsigned int i = 0; i< _segments + 1; i++)  
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

void ShapeRenderable::buildSphereLines()
{
	SphereShape *sphere = static_cast<SphereShape *>(_shape);

	unsigned int ring = _segments / 2;
	float deltaRing = MATH_PI / (float)(ring);
	float deltaSegment = 2.0f * MATH_PI / (float)_segments;
	float deltaV = 1.0f / (float)(ring);
	float deltaU = 1.0f / (float)_segments;

	Vec3Array vertexArray;
	Vec4Array colorArray;
	UIntArray indexArray;
	for(unsigned int j = 0; j < ring + 1; j++)  
	{  
		float radius = sinf(j * deltaRing);
		float y0 = cosf(MATH_PI - j * deltaRing);

		for (unsigned int i = 0; i < _segments + 1; i++)  
		{  
			float x0 = radius * sinf(i * deltaSegment);  
			float z0 = radius * cosf(i * deltaSegment);  

			glm::vec4 v = glm::vec4(glm::vec3(x0, y0, z0) * sphere->getRadius(), 1.0f);
			vertexArray.push_back((sphere->getOriginMat() * v).xyz);
			colorArray.push_back(glm::vec4(1.0f));

			unsigned int vIndex = vertexArray.size() - 1;
			if (i != _segments)
			{
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + 1);
			}
			if (j != ring) 
			{  
				indexArray.push_back(vIndex);
				indexArray.push_back(vIndex + _segments + 1);
			}  
		}  
	} 

	Geometry::IndicesArray indices;
	indices.push_back(std::make_pair(Geometry::GT_LINES, indexArray));
	_geometry->setVertexArray(vertexArray);
	_geometry->setColorArray(colorArray);
	_geometry->setIndicesArray(indices);
}

void ShapeRenderable::buildBoxLines()
{
	BoxShape *box = static_cast<BoxShape *>(_shape);

	Vec3Array vertexArray;
	Vec4Array colorArray;
	vertexArray.push_back((box->getOriginMat() * glm::vec4(-box->getHalfLength(), -box->getHalfHeight(), -box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(box->getHalfLength(), -box->getHalfHeight(), -box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(box->getHalfLength(), box->getHalfHeight(), -box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(-box->getHalfLength(), box->getHalfHeight(), -box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(-box->getHalfLength(), -box->getHalfHeight(), box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(box->getHalfLength(), -box->getHalfHeight(), box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(box->getHalfLength(), box->getHalfHeight(), box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));
	vertexArray.push_back((box->getOriginMat() * glm::vec4(-box->getHalfLength(), box->getHalfHeight(), box->getHalfWidth(), 1.0f)).xyz); colorArray.push_back(glm::vec4(1.0f));

	UIntArray indexArray;
	indexArray.push_back(0);indexArray.push_back(1);
	indexArray.push_back(1);indexArray.push_back(2);
	indexArray.push_back(2);indexArray.push_back(3);
	indexArray.push_back(3);indexArray.push_back(0);
	indexArray.push_back(4);indexArray.push_back(5);
	indexArray.push_back(5);indexArray.push_back(6);
	indexArray.push_back(6);indexArray.push_back(7);
	indexArray.push_back(7);indexArray.push_back(4);
	indexArray.push_back(0);indexArray.push_back(4);
	indexArray.push_back(1);indexArray.push_back(5);
	indexArray.push_back(2);indexArray.push_back(6);
	indexArray.push_back(3);indexArray.push_back(7);

	Geometry::IndicesArray indices;
	indices.push_back(std::make_pair(Geometry::GT_LINES, indexArray));
	_geometry->setVertexArray(vertexArray);
	_geometry->setColorArray(colorArray);
	_geometry->setIndicesArray(indices);
}

void ShapeRenderable::buildCylinderLines()
{

}

void ShapeRenderable::buildCapsuleLines()
{

}
