#include "Geometry.h"
#include "Program.h"
#include "Renderer.h"
#include "Camera.h"
#include "Material.h"

Geometry::Geometry()
	: _VAO(0)
	, _needGenerateVAO(true)
	, _material(new Material)
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
		_material->apply(camera->getProjectionMatrix(), camera->getViewMatrix());
	}

	glBindVertexArray(_VAO);
		unsigned int offset = 0;
		for (auto iter : _indices){
			glDrawElements(iter.first, iter.second.size(), GL_UNSIGNED_INT, (GLvoid *)(offset));
			offset += iter.second.size() * sizeof(GLuint);
		}	
	glBindVertexArray(0);
}

void Geometry::setVertexArray( const Vec3Array &ary )
{
	_vertices = ary;
	generateVertexVBO();
}

void Geometry::setNormalArray( const Vec3Array &ary )
{
	_normals = ary;
	generateNormalVBO();
}

void Geometry::setTangentArray( const Vec3Array &ary )
{
	_tangents = ary;
	generateTangentVBO();
}

void Geometry::setBitangentArray( const Vec3Array &ary )
{
	_bitangents = ary;
	generateBitangentVBO();
}

void Geometry::setColorArray( const Vec4Array &ary )
{
	_colors = ary;
	generateColorVBO();
}

void Geometry::setTextureCoordsArray( const TextureCoordsArray &ary )
{
	_textureCoords = ary;
	generateTextureCoordVBO();
}

void Geometry::setIndicesArray( const IndicesArray &ary )
{
	_indices = ary;
	generateIndexVBO();
}

void Geometry::generateVertexVBO()
{
	if (!_vertices.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[0] == 0)
			glGenBuffers(1, &_VBOS[0]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[0]);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}
}

void Geometry::generateNormalVBO()
{
	if (!_normals.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[1] == 0)
			glGenBuffers(1, &_VBOS[1]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[1]);
		glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals[0]), &_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}
}

void Geometry::generateTangentVBO()
{
	if (!_tangents.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[4] == 0)
			glGenBuffers(1, &_VBOS[4]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[4]);
		glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(_tangents[0]), &_tangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);
	}
}

void Geometry::generateBitangentVBO()
{
	if (!_bitangents.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[5] == 0)
			glGenBuffers(1, &_VBOS[5]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[5]);
		glBufferData(GL_ARRAY_BUFFER, _bitangents.size() * sizeof(_bitangents[0]), &_bitangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(5);
	}
}

void Geometry::generateColorVBO()
{
	if (!_colors.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[3] == 0)
			glGenBuffers(1, &_VBOS[3]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBOS[3]);
		glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(_colors[0]), &_colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
	}
}

void Geometry::generateTextureCoordVBO()
{
	if (!_textureCoords.empty()){
		glBindVertexArray(_VAO);
		if (!_textureCoords[0].empty()){
			if (_VBOS[2] == 0)
				glGenBuffers(1, &_VBOS[2]);
			glBindBuffer(GL_ARRAY_BUFFER, _VBOS[2]);
			glBufferData(GL_ARRAY_BUFFER, _textureCoords[0].size() * sizeof(_textureCoords[0][0]), &_textureCoords[0][0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(2);
		}
	}
}

void Geometry::generateIndexVBO()
{
	if (!_indices.empty()){
		glBindVertexArray(_VAO);
		if (_VBOS[6] == 0)
			glGenBuffers(1, &_VBOS[6]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBOS[6]);
		unsigned int bufSize = 0;
		for (auto iter : _indices){
			bufSize += iter.second.size() * sizeof(GLuint);	
		}  
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, nullptr, GL_STATIC_DRAW);

		unsigned int offset = 0;
		for (auto iter : _indices){
			if (!iter.second.empty()){
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, iter.second.size() * sizeof(GLuint), &(iter.second[0]));
				offset += iter.second.size() * sizeof(GLuint);
			}
		}
	}
}