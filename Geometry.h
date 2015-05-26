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

	enum DataStorage
	{
		DS_STATIC = GL_STATIC_DRAW,
		DS_DYNAMIC = GL_DYNAMIC_DRAW,
		DS_STREAM = GL_STREAM_DRAW
	};

	enum class MapType
	{
		MT_VERTEX,
		MT_NORMAL,
		MT_TANGENT,
		MT_BITANGENT,
		MT_COLOR,
		MT_TEXCOORD0,
		MT_INDEX = MT_TEXCOORD0 + 0x10
	};

	typedef std::vector<Vec4Array> TextureCoordsArray;
	typedef std::vector<std::pair<PrimitiveType, UIntArray> > IndicesArray;

	Geometry();
	virtual ~Geometry();

	virtual void draw(const Renderer *renderer, const glm::mat4 &modelMatrix) override;

	virtual void setMaterial(Material *material) override { _material = material; };
	virtual Material* getMaterial() const override { return _material; };

	virtual BoundingSphere* getBoundingSphere() override;
	virtual BoundingBox* getBoundingBox() override;

	void setVertexArray(const Vec3Array &ary, DataStorage usage = DS_STATIC);
	void setNormalArray(const Vec3Array &ary, DataStorage usage = DS_STATIC);
	void setTangentArray(const Vec3Array &ary, DataStorage usage = DS_STATIC);
	void setBitangentArray(const Vec3Array &ary, DataStorage usage = DS_STATIC);
	void setColorArray(const Vec4Array &ary, DataStorage usage = DS_STATIC);
	void setTextureCoordsArray(const TextureCoordsArray &ary, DataStorage usage = DS_STATIC);
	void setIndicesArray(const IndicesArray &ary, DataStorage usage = DS_STATIC);
	void setDrawArrays(PrimitiveType pt, unsigned int first,unsigned int count);

	void* mapArray(MapType mt);
	void unMapArray(MapType mt);

	const Vec3Array& getVertexArray() const { return _vertices; };
	const Vec3Array& getNormalArray() const { return _normals; };
	const Vec3Array& getTangentArray() const { return _tangents; };
	const Vec3Array& getBitangentArray() const { return _bitangents; };
	const Vec4Array& getColorArray() const { return _colors; };
	const TextureCoordsArray& getTextureCoordsArray() const { return _textureCoords; };
	const IndicesArray& getIndicesArray() const { return _indices; };

	void dirty();

protected:

	void generateVertexVBO(DataStorage usage);
	void generateNormalVBO(DataStorage usage);
	void generateTangentVBO(DataStorage usage);
	void generateBitangentVBO(DataStorage usage);
	void generateColorVBO(DataStorage usage);
	void generateTextureCoordVBO(DataStorage usage);
	void generateIndexVBO(DataStorage usage);
	void caculateBoundingSphere();
	void caculateBoundingBox();

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
	bool _dirtyBS;
	bool _dirtyBB;

	bool _useDrawArrays;
	PrimitiveType _primitiveType;
	unsigned int _first;
	unsigned int _count;

	Material *_material;
	BoundingSphere *_boundingSphere;
	BoundingBox *_boundingBox;
};

