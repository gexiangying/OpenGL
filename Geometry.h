#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "RenderableObject.h"
#include "Array.h"

class Material;
class Geometry :
	public RenderableObject
{
public:
	enum PrimitiveType
	{
		GT_POINTS = GL_POINTS,
		GT_LINES = GL_LINES,
		GT_LINE_STRIP = GL_LINE_STRIP,
		GT_LINE_LOOP = GL_LINE_LOOP,
		GT_TRIANGLES = GL_TRIANGLES,
		GT_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		GT_TRIANGLE_FAN = GL_TRIANGLE_FAN
	};

	typedef std::vector<Vec4Array> TextureCoordsArray;
	typedef std::vector<std::pair<PrimitiveType, UIntArray> > IndicesArray;

	Geometry();
	virtual ~Geometry();

	virtual void draw(const Renderer *renderer, const glm::mat4 &modelMatrix) override;

	virtual void setMaterial(Material *material) { _material = material; };
	virtual Material* getMaterial() const { return _material; };

	void setVertexArray(const Vec3Array &ary);
	void setNormalArray(const Vec3Array &ary);
	void setTangentArray(const Vec3Array &ary);
	void setBitangentArray(const Vec3Array &ary);
	void setColorArray(const Vec4Array &ary);
	void setTextureCoordsArray(const TextureCoordsArray &ary);
	void setIndicesArray(const IndicesArray &ary);

	const Vec3Array& getVertexArray() const { return _vertices; };
	const Vec3Array& getNormalArray() const { return _normals; };
	const Vec3Array& getTangentArray() const { return _tangents; };
	const Vec3Array& getBitangentArray() const { return _bitangents; };
	const Vec4Array& getColorArray() const { return _colors; };
	const TextureCoordsArray& getTextureCoordsArray() const { return _textureCoords; };
	const IndicesArray& getIndicesArray() const { return _indices; };


protected:

	void generateVertexVBO();
	void generateNormalVBO();
	void generateTangentVBO();
	void generateBitangentVBO();
	void generateColorVBO();
	void generateTextureCoordVBO();
	void generateIndexVBO();

protected:

	Vec3Array _vertices;
	Vec3Array _normals;
	Vec3Array _tangents;
	Vec3Array _bitangents;
	Vec4Array _colors;
	TextureCoordsArray _textureCoords;
	IndicesArray _indices;

	GLuint _VAO;
	GLuint _VBOS[7];
	bool _needGenerateVAO;

	Material *_material;
};

