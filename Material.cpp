#include "Material.h"
#include "Program.h"
#include "TextureManager.h"

static const GLchar *Material_V_Shader[] = {
	"#version 430                                                    \n"
	"layout (location = 0) in vec3 position;                        \n"
	"layout (location = 1) in vec3 normal;                          \n"
	"layout (location = 2) in vec4 texcoord;                         \n"
	"layout (location = 3) in vec4 color;                         \n"
	"uniform mat4 u_ModelViewProjectMat;                              \n"
	"uniform mat4 u_ModelViewMat;                                     \n"
	"uniform mat3 u_NormalMat;                                       \n"
	"out vec3 v_position;                                             \n"
	"out vec3 v_normal;                                             \n"
	"out vec2 v_texcoord;                                             \n"
	"out vec4 v_color;                                             \n"
	"void main()                                                     \n"
	"{                                                               \n"
	"	v_normal = u_NormalMat * normal;                              \n"
	"	v_position = (u_ModelViewMat * vec4(position, 1.0)).xyz;         \n"
	"	v_texcoord = texcoord.xy;                             \n"
	"	v_color = color;                             \n"
	"	gl_Position = u_ModelViewProjectMat * vec4(position, 1.0);                 \n"
	"}                                                               \n"
};

static const GLchar *Material_F_Shader[] = {
	"#version 430                        \n"
	"uniform vec4 u_diffuseColor;                                       \n"
	"uniform vec4 u_specularColor;                                       \n"
	"uniform vec4 u_ambientColor;                                       \n"
	"uniform float u_shininess;                                       \n"
	"in vec3 v_position;                                             \n"
	"in vec3 v_normal;                                             \n"
	"in vec2 v_texcoord;                                             \n"
	"in vec4 v_color;                                             \n"
	"out vec4 color;                        \n"
	"void main(){                        \n"
	"	vec3 lDir = normalize(vec3(1.0, 1.0, 1.0));                       \n"
	"	vec3 normal = normalize(v_normal);                        \n"
	"	vec3 eye = normalize(-v_position);                       \n"
	"	vec3 H = normalize(eye + lDir);                       \n"
	"	float specfactor = pow(max(0.0, dot(normal, H)), u_shininess);                       \n"
	"	float diffactor = max(0.0, dot(normal, lDir));                        \n"
	"	color = diffactor * u_diffuseColor + specfactor * u_specularColor + u_ambientColor;    \n"
	"}                        \n"
};


Material::Material()
	: _diffuseColor(glm::vec4(1.0f))
	, _specularColor(glm::vec4(0.2f))
	, _amibientColor(glm::vec4(0.1f))
	, _emissionColor(glm::vec4(0.0f))
	, _shininess(8.0f)
	, _diffuseTexture(0)
	, _specularTexture(0)
	, _ambientTexture(0)
{
	_program = new Program;
	_program->attachShader(GL_VERTEX_SHADER, Material_V_Shader);
	_program->attachShader(GL_FRAGMENT_SHADER, Material_F_Shader);
	_program->link();
	_ambientTexture = _specularTexture = _diffuseTexSampler = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_REPEAT);
}


Material::~Material()
{
}

void Material::apply( const glm::mat4 &projection, const glm::mat4 &modelView )
{
	if (_program){
		if (!_program->isLinked()){
			_program->link();
		}
		_program->setUniform4f("u_diffuseColor", _diffuseColor);
		_program->setUniform4f("u_specularColor", _specularColor);
		_program->setUniform4f("u_ambientColor", _amibientColor);
		_program->setUniformf("u_shininess", _shininess);
		if (_diffuseTexture){
			glActiveTexture(GL_TEXTURE0);
			TextureManager::Instance()->BindTexture(_diffuseTexture);
			TextureManager::Instance()->BindSampler(0, _diffuseTexSampler);
			_program->setUniformi("u_diffuseTex", 0);
		}
		if (_specularTexture){
			glActiveTexture(GL_TEXTURE1);
			TextureManager::Instance()->BindTexture(_specularTexture);
			TextureManager::Instance()->BindSampler(1, _specularTexSampler);
			_program->setUniformi("u_specularTex", 1);
		}
		if (_ambientTexture){
			glActiveTexture(GL_TEXTURE2);
			TextureManager::Instance()->BindTexture(_ambientTexture);
			TextureManager::Instance()->BindSampler(2, _ambientTexSampler);
			_program->setUniformi("u_ambientTex", 2);
		}
		_program->apply(projection, modelView);
	}
}
