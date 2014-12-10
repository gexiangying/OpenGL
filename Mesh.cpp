#include "Mesh.h"

static const GLchar *Mesh_V_ShaderSource[] = {
	"#version 430                                                    \n"
	"layout (location = 0) in vec3 position;                        \n"
	"layout (location = 1) in vec3 normal;                          \n"
	//"layout (location = 2) in ivec4 connection;                   \n"
	"uniform mat4 u_ModelViewProjectMat;                                          \n"
	"uniform mat3 u_NormalMat;                                       \n"
	"out vec3 v_normal;                                             \n"
	"void main()                                                     \n"
	"{                                                               \n"
	"	v_normal = u_NormalMat * normal;                              \n"
	"	gl_Position = u_ModelViewProjectMat * vec4(position, 1.0);                 \n"
	"}                                                               \n"
};

static const GLchar *Mesh_F_ShaderSource[] = {
	"#version 430                        \n"
	"in vec3 v_normal;                                             \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	vec3 normal = normalize(v_normal);                        \n"
	//"	float factor = max(0.0, dot(normal, normalize(vec3(1.0, 0.0, 1.0)))); \n"
	"	color = vec4(normal, 1.0);                        \n"
	"}                        \n"
};


Mesh::Mesh(aiMesh *mesh, aiMaterial *material)
	: _data(mesh)
	, _material(material)
	, _drawElementCount(0)
	, _drawType(GL_TRIANGLES)
	, _program(nullptr)
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	fillVertices();
	fillIndices();
	glBindVertexArray(0);
	createShaderProgram();
}


Mesh::~Mesh()
{
}

void Mesh::draw(const glm::mat4x4 &modelView)
{
	if (0 < _drawElementCount){
		_program->apply(modelView);
		glBindVertexArray(_VAO);
			glDrawElements(_drawType, _drawElementCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}

void Mesh::fillVertices()
{
	GLuint vbo;
	if (_data->HasPositions()){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}

	if (_data->HasNormals()){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mNormals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}

	if (_data->HasTangentsAndBitangents()){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mTangents, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mBitangents, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
	}

	if (_data->HasTextureCoords(0)){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mTextureCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);
	}

	if (_data->HasVertexColors(0)){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiColor4D), _data->mColors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(5);
	}

}

void Mesh::fillIndices()
{
	if (_data->HasFaces()){
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < _data->mNumFaces; ++i){
			const aiFace &face = _data->mFaces[i];
			for (unsigned int n = 0; n < face.mNumIndices; ++n){
				indices.push_back(face.mIndices[n]);
			}
		}

		switch((_data->mFaces[0]).mNumIndices) {
		case 1: _drawType = GL_POINTS; break;
		case 2: _drawType = GL_LINES; break;
		case 3: _drawType = GL_TRIANGLES; break;
		default: _drawType = GL_POINTS; break;
		}

		_drawElementCount = indices.size();

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	}
}

void Mesh::createShaderProgram()
{
	if (!_program) _program = new Program;
	_program->attachShader(GL_VERTEX_SHADER, Mesh_V_ShaderSource);
	_program->attachShader(GL_FRAGMENT_SHADER, Mesh_F_ShaderSource);
	_program->link();
}

void Mesh::setProgram( Program *program )
{
	_program = program;
}
