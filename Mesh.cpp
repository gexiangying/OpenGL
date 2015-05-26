#include "Mesh.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Camera.h"

static const GLchar *Mesh_V_ShaderSource[] = {
	"#version 430                                                    \n"
	"layout (location = 0) in vec3 position;                        \n"
	"layout (location = 1) in vec3 normal;                          \n"
	"layout (location = 2) in vec2 texcoord;                         \n"
	"uniform mat4 u_ModelViewProjectMat;                              \n"
	"uniform mat4 u_ModelViewMat;                                     \n"
	"uniform mat3 u_NormalMat;                                       \n"
	"out vec3 v_position;                                             \n"
	"out vec3 v_normal;                                             \n"
	"out vec2 v_texcoord;                                             \n"
	"void main()                                                     \n"
	"{                                                               \n"
	"	v_normal = u_NormalMat * normal;                              \n"
	"	v_position = (u_ModelViewMat * vec4(position, 1.0)).xyz;         \n"
	"	v_texcoord = texcoord;                             \n"
	"	gl_Position = u_ModelViewProjectMat * vec4(position, 1.0);                 \n"
	"}                                                               \n"
};

static const GLchar *Mesh_F_ShaderSource[] = {
	"#version 430                        \n"
	"uniform vec4 u_diffuseColor;                                       \n"
	"uniform vec4 u_specularColor;                                       \n"
	"uniform vec4 u_ambientColor;                                       \n"
	"uniform float u_shininess;                                       \n"
	"uniform sampler2D u_diffuseTex;                                       \n"
	"in vec3 v_position;                                             \n"
	"in vec3 v_normal;                                             \n"
	"in vec2 v_texcoord;                                             \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	vec3 lDir = normalize(vec3(1.0, 1.0, 1.0));                       \n"
	"	vec3 normal = normalize(v_normal);                        \n"
	"	vec3 eye = normalize(-v_position);                       \n"
	"	vec3 H = normalize(eye + lDir);                       \n"
	"	float specfactor = pow(max(0.0, dot(normal, H)), u_shininess);                       \n"
	"	float diffactor = max(0.0, dot(normal, lDir));                        \n"
	"	color = texture(u_diffuseTex, v_texcoord) * diffactor * u_diffuseColor + specfactor * u_specularColor;    \n"
	"}                        \n"
};


Mesh::Mesh(aiMesh *mesh, aiMaterial *material, const std::string &path)
	: _data(mesh)
	, _material(material)
	, _drawElementCount(0)
	, _drawType(GL_TRIANGLES)
	, _program(nullptr)
	, _folderPath(path)
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	fillVertices();
	fillIndices();
	glBindVertexArray(0);
	createShaderProgram();
	initMaterial();
}


Mesh::~Mesh()
{
}

void Mesh::draw(const Renderer *renderer, const glm::mat4 &modelMatrix)
{
	if (0 < _drawElementCount){
		auto camera = renderer->getCamera();

		if (!_program->isLinked()){
			_program->link();
			initMaterial();
		}
		_program->apply(camera->getProjectionMatrix(), camera->getViewMatrix() * modelMatrix);

		for (auto iter : _textureUnitMap){
			glActiveTexture(GL_TEXTURE0 + iter.first);
			TextureManager::Instance()->BindTexture(iter.second.first);
			TextureManager::Instance()->BindSampler(iter.first, iter.second.second);
		}

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

	if (_data->HasTextureCoords(0)){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mTextureCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
	}

	if (_data->HasVertexColors(0)){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiColor4D), _data->mColors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
	}

	if (_data->HasTangentsAndBitangents()){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mTangents, GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _data->mNumVertices * sizeof(aiVector3D), _data->mBitangents, GL_STATIC_DRAW);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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
	if (_program->isLinked())
		initMaterial();
}

void Mesh::initMaterial()
{
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	float shininess = 0.0f;
	unsigned int max = 1;

	aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(_material, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialColor(_material, AI_MATKEY_COLOR_AMBIENT, &ambient);
	aiGetMaterialFloatArray(_material, AI_MATKEY_SHININESS, &shininess, &max);
	_program->setUniform4f("u_diffuseColor", glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	_program->setUniform4f("u_specularColor", glm::vec4(specular.r, specular.g, specular.b, specular.a));
	_program->setUniform4f("u_ambientColor", glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a));
	_program->setUniformf("u_shininess", shininess);

	initDiffuse();
}

void Mesh::initDiffuse()
{
	aiString texPath;
	GLuint texUnit = 0;
	int type = aiTextureMapMode_Wrap;
	_material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
	if (texPath.length > 0)
		TextureManager::Instance()->LoadTexture(texUnit, (_folderPath + std::string(texPath.C_Str())).c_str());
	else
		TextureManager::Instance()->LoadTexture(texUnit);
	TextureManager::Instance()->BindTexture(texUnit);
	_program->setUniformi("u_diffuseTex", 0);
	_textureUnitMap[0].first = texUnit;


	_material->Get(AI_MATKEY_UVWSRC_DIFFUSE(0), type);
	switch (type)
	{
	case aiTextureMapMode_Wrap:
		_textureUnitMap[0].second = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_REPEAT);
		break;

	case aiTextureMapMode_Clamp:
		_textureUnitMap[0].second = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP_TO_BORDER);
		break;

	case aiTextureMapMode_Decal:
		_textureUnitMap[0].second = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP);
		break;

	case aiTextureMapMode_Mirror:
		_textureUnitMap[1].second = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_MIRRORED_REPEAT);
		break;
	default:
		break;
	}
}
