#include "Geometry.h"
#include "Program.h"
#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

Geometry::Geometry()
	: _VAO(0)
	, _dirtyBS(true)
	, _dirtyBB(true)
	, _material(new Material)
	, _boundingSphere(new BoundingSphere(glm::vec3(), 0.0f))
	, _boundingBox(new BoundingBox)
	, _useDrawArrays(false)
	, _primitiveType(GT_POINTS)
	, _first(0)
	, _count(0)
{
	memset(_VBOS, 0, sizeof(_VBOS));

	glGenVertexArrays(1, &_VAO);
}


Geometry::~Geometry()
{
}

void Geometry::draw( const Renderer *renderer, const glm::mat4 &modelMatrix )
{
	auto camera = renderer->getCamera();

	if (_material){
		_material->apply(camera->getProjectionMatrix(), camera->getViewMatrix() * modelMatrix);
	}

	glBindVertexArray(_VAO);
	if (_useDrawArrays){
		glDrawArrays(_primitiveType, _first, _count);
	}else{
		unsigned int offset = 0;
		for (const auto &iter : _indices){
			glDrawElements(iter.first, iter.second.size(), GL_UNSIGNED_INT, (GLvoid *)(offset));
			offset += iter.second.size() * sizeof(GLuint);
		}	
	}
	glBindVertexArray(0);
}

void Geometry::setVertexArray( const Vec3Array &ary, DataStorage usage )
{
	_vertices = ary;
	generateVertexVBO(usage);
	dirty();
}

void Geometry::setNormalArray( const Vec3Array &ary, DataStorage usage )
{
	_normals = ary;
	generateNormalVBO(usage);
	dirty();
}

void Geometry::setTangentArray( const Vec3Array &ary, DataStorage usage )
{
	_tangents = ary;
	generateTangentVBO(usage);
	dirty();
}

void Geometry::setBitangentArray( const Vec3Array &ary, DataStorage usage )
{
	_bitangents = ary;
	generateBitangentVBO(usage);
	dirty();
}

void Geometry::setColorArray( const Vec4Array &ary, DataStorage usage )
{
	_colors = ary;
	generateColorVBO(usage);
	dirty();
}

void Geometry::setTextureCoordsArray( const TextureCoordsArray &ary, DataStorage usage )
{
	_textureCoords = ary;
	generateTextureCoordVBO(usage);
	dirty();
}

void Geometry::setIndicesArray( const IndicesArray &ary, DataStorage usage )
{
	_indices = ary;
	_useDrawArrays = false;
	generateIndexVBO(usage);
	dirty();
}

void Geometry::generateVertexVBO(DataStorage usage)
{
	if (!_vertices.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[0] == 0)
			glGenBuffers(1, &_VBOS[0]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[0]);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), &_vertices[0], usage);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}
}

void Geometry::generateNormalVBO(DataStorage usage)
{
	if (!_normals.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[1] == 0)
			glGenBuffers(1, &_VBOS[1]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[1]);
		glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals[0]), &_normals[0], usage);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}
}

void Geometry::generateTangentVBO(DataStorage usage)
{
	if (!_tangents.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[4] == 0)
			glGenBuffers(1, &_VBOS[4]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[4]);
		glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(_tangents[0]), &_tangents[0], usage);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);
	}
}

void Geometry::generateBitangentVBO(DataStorage usage)
{
	if (!_bitangents.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[5] == 0)
			glGenBuffers(1, &_VBOS[5]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[5]);
		glBufferData(GL_ARRAY_BUFFER, _bitangents.size() * sizeof(_bitangents[0]), &_bitangents[0], usage);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(5);
	}
}

void Geometry::generateColorVBO(DataStorage usage)
{
	if (!_colors.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[3] == 0)
			glGenBuffers(1, &_VBOS[3]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[3]);
		glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(_colors[0]), &_colors[0], usage);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
	}
}

void Geometry::generateTextureCoordVBO(DataStorage usage)
{
	if (!_textureCoords.empty()){
		glBindVertexArray(_VAO);
		if (!_textureCoords[0].empty()){
			if (_VBOS[2] == 0)
				glGenBuffers(1, &_VBOS[2]);
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[2]);
			glBufferData(GL_ARRAY_BUFFER, _textureCoords[0].size() * sizeof(_textureCoords[0][0]), &_textureCoords[0][0], usage);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(2);
		}
	}
}

void Geometry::generateIndexVBO(DataStorage usage)
{
	if (!_indices.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[6] == 0)
			glGenBuffers(1, &_VBOS[6]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBOS[6]);
		unsigned int bufSize = 0;
		for (const auto &iter : _indices){
			bufSize += iter.second.size() * sizeof(GLuint);	
		}  
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, nullptr, GL_STATIC_DRAW);

		unsigned int offset = 0;
		for (const auto &iter : _indices){
			if (!iter.second.empty()){
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, iter.second.size() * sizeof(GLuint), &(iter.second[0]));
				offset += iter.second.size() * sizeof(GLuint);
			}
		}
	}
}

BoundingSphere* Geometry::getBoundingSphere() 
{
	if (_dirtyBS){
		caculateBoundingSphere();
		_dirtyBS = false;
	}
	return _boundingSphere;
}

BoundingBox* Geometry::getBoundingBox()
{
	if (_dirtyBB){
		caculateBoundingBox();
		_dirtyBB = false;
	}
	return _boundingBox;
}

void Geometry::caculateBoundingSphere()
{
	//Ritter, Jack, ¡°An Efficient Bounding Sphere,¡± in Andrew S. Glassner, ed.
	//Graphics Gems, Academic Press, pp. 301¨C303, 1990. http://www.graphicsgems.org Citedon
	unsigned int vInxMin = 0,vInxMax = 0,vInyMin = 0,vInyMax = 0,vInzMin = 0,vInzMax = 0;

	for (unsigned int i = 1; i < _vertices.size(); ++i){
		if (_vertices[vInxMin].x > _vertices[i].x) vInxMin = i;
		else if (_vertices[vInxMax].x < _vertices[i].x) vInxMax = i;

		if (_vertices[vInyMin].y > _vertices[i].y) vInyMin = i;
		else if (_vertices[vInyMax].y < _vertices[i].y) vInyMax = i;

		if (_vertices[vInzMin].z > _vertices[i].z) vInzMin = i;
		else if (_vertices[vInzMax].z < _vertices[i].z) vInzMax = i;
	}
	glm::vec3 center;
	float radius = 0.0f;
	float xLen = glm::length(_vertices[vInxMax] - _vertices[vInxMin]);
	float yLen = glm::length(_vertices[vInyMax] - _vertices[vInyMin]);
	float zLen = glm::length(_vertices[vInzMax] - _vertices[vInzMin]);
	if (yLen <= xLen && zLen <= xLen){
		center = (_vertices[vInxMax] + _vertices[vInxMin]) * 0.5f;
		radius = xLen * 0.5f;
	}else if (xLen <= yLen && zLen <= yLen){
		center = (_vertices[vInyMax] + _vertices[vInyMin]) * 0.5f;
		radius = yLen * 0.5f;
	}else if (xLen <= zLen && yLen <= zLen){
		center = (_vertices[vInzMax] + _vertices[vInzMin]) * 0.5f;
		radius = zLen * 0.5f;
	}

	for (const auto &iter : _vertices){
		float dis = glm::length(iter - center);
		if (radius < dis){
			glm::vec3 dir = (iter - center) / dis;
			center = center + dir * (dis - radius) * 0.5f;
			radius = (dis + radius) * 0.5f;
		}
	}

	_boundingSphere->setCenter(center);
	_boundingSphere->setRadius(radius);
}

void Geometry::caculateBoundingBox()
{
	for (const auto &iter : _vertices){
		_boundingBox->expandBy(iter);
	}
}

void Geometry::dirty()
{
	_dirtyBS = _dirtyBB = true;
}

void* Geometry::mapArray( MapType mt )
{
	void *buf = nullptr;
	switch (mt)
	{
	case MapType::MT_VERTEX:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[0]);
			buf = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	case MapType::MT_NORMAL:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[1]);
			buf = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	case MapType::MT_TANGENT:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[4]);
			buf = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	case MapType::MT_BITANGENT:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[5]);
			buf = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	case MapType::MT_COLOR:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[3]);
			buf = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	case MapType::MT_TEXCOORD0:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[2]);
			buf = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	case MapType::MT_INDEX:
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBOS[6]);
			buf = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
		}
		break;

	default:
		break;
	}
	return buf;
}

void Geometry::unMapArray(MapType mt)
{
	switch (mt)
	{
	case MapType::MT_VERTEX:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[0]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		break;

	case MapType::MT_NORMAL:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[1]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		break;

	case MapType::MT_TANGENT:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[4]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		break;

	case MapType::MT_BITANGENT:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[5]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		break;

	case MapType::MT_COLOR:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[3]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		break;

	case MapType::MT_TEXCOORD0:
		{
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[2]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		break;

	case MapType::MT_INDEX:
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBOS[6]);
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
		break;

	default:
		break;
	}
}

void Geometry::setDrawArrays( PrimitiveType pt, unsigned int first,unsigned int count )
{
	_primitiveType = pt;
	_first = first;
	_count = count;
	_useDrawArrays = true;
	dirty();
}
