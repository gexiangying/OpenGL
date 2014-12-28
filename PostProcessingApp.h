#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "Model.h"
#include "TextureManager.h"
#include "FrameBufferObject.h"
#include "Program.h"
#include "Displayer.h"
#include "RenderQuad.h"
#include <iostream>
static double PI = 3.14159265358979323846264338327950288;
static double HALF_PI = PI / 2.0;
static double TWO_PI = PI * PI;


double gaussFunc(double x, double sigma2){
	double twopi = PI * PI;
	double a = 1.0 / sqrt(twopi * sigma2);

	return a * exp((-x * x) / (2.0 * sigma2));
}

float jitter()
{
	return ((float)rand() / RAND_MAX) -0.5f;
}

GLuint CreateRandomPointsImageInSphereSurface(unsigned int sampleU, unsigned int sampleV)
{
	unsigned int tWidth = sampleU;
	unsigned int tHeight = sampleV;
	unsigned int samples = sampleU * sampleV;
	unsigned int tDepth = samples;

	float* imgdata = new float[tWidth * tHeight * tDepth * 3];

	unsigned int idx = 0;
	for (unsigned int w = 0; w < tWidth; ++w)
	{
		for (unsigned int h = 0; h < tHeight; ++h)
		{
			for (unsigned int d = 0; d < tDepth; ++d)
			{
				unsigned int x1,y1;
				x1 = d % sampleU;
				y1 = (samples - 1 - d) / sampleU;
				glm::vec2 v = glm::vec2(x1 + 0.5 + jitter(), y1 + 0.5 + jitter());

				v.x /= sampleU;
				v.y /= sampleV;

				float phi = 2 * jitter();
				imgdata[idx    ] = v.y * cosf(HALF_PI * phi) * cosf(TWO_PI * v.x);
				imgdata[idx + 1] = v.y * sinf(HALF_PI * phi);
				imgdata[idx + 2] = v.y * cosf(HALF_PI * phi) * sinf(TWO_PI * v.x);
				idx += 3;
				//std::cout<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<std::endl;
			}
		}
	}

	GLuint texID = TextureManager::Instance()->CreateTexture(tWidth, tHeight, tDepth, GL_RGB32F, imgdata);

	delete[] imgdata;
	return texID;
};

//////////////////////////////////////////////////////////////////////////

class PostProcessingApp : public Application{
public:

	virtual void Render(double elapseTime) override {

		glm::ivec2 size = Displayer::Instance()->GetWindowSize();

		_renderer.render();

		for (auto iter : _rttRenders)
			iter->render();
	}

	virtual void Setup() override {
		SSAOSetup();
	}

	void BloomSetup(){
		auto model = new Model("datas/teapot.obj");
		_renderer.attachRenderableObject(model);
		_renderer.setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		_renderer.depthTest(true);

		glm::ivec2 size = Displayer::Instance()->GetWindowSize();
		_rttTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		auto fbo = new FrameBufferObject;
		fbo->attachTexture(GL_COLOR_ATTACHMENT0, _rttTexture);
		_renderer.renderToFrameBuferObject(fbo);

		auto elTex = ExtractBright(_rttTexture, 0.2f, size);
		auto blurTex = GaussBlur(elTex, size);

		//////////////////////////////////////////////////////////////////////////
		static const GLchar *V_ShaderSource[] = {
			"#version 430                        \n"
			"layout(location = 0) in vec4 position;                       \n"
			"layout(location = 1) in vec2 uv;                       \n"
			"uniform mat4 u_ModelViewProjectMat;                                      \n"
			"out vec2 v_uv;                                          \n"
			"void main(){                        \n"
			"	v_uv = uv;                                \n"
			"	gl_Position = u_ModelViewProjectMat * position;                        \n"
			"}                        \n"
		};

		static const GLchar *F_ShaderSource[] = {
			"#version 430                        \n"
			"uniform sampler2D u_Texture0;                       \n"
			"uniform sampler2D u_Texture1;                       \n"
			"in vec2 v_uv;                                          \n"
			"out vec4 color;                        \n"
			"void main(){                        \n"
			"	color = texture(u_Texture0, v_uv) + texture(u_Texture1, v_uv);     \n"
			"}                        \n"
		};

		auto rttRender = new Renderer();
		rttRender->setClearColor(glm::vec4(1.0f));
		rttRender->depthTest(false);
		RenderQuad *rq = new RenderQuad(size.x, size.y);
		auto program = new Program;
		program->attachShader(GL_VERTEX_SHADER, V_ShaderSource);
		program->attachShader(GL_FRAGMENT_SHADER, F_ShaderSource);
		rq->setProgram(program);
		rq->setTexture(0, _rttTexture, TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP));
		rq->setTexture(1, blurTex, TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP));
		rttRender->attachRenderableObject(rq);
		_rttRenders.push_back(rttRender);
	}

	void SSAOSetup(){
		glm::ivec2 size = Displayer::Instance()->GetWindowSize();
		auto model = new Model("datas/teapot.obj");
		auto colorTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		auto posTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		auto norm_depthTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		_renderer.setClearColor(glm::vec4(1.0f));
		_renderer.depthTest(true);
		auto fbo = new FrameBufferObject;
		fbo->attachTexture(GL_COLOR_ATTACHMENT0, colorTexture);
		fbo->attachTexture(GL_COLOR_ATTACHMENT1, posTexture);
		fbo->attachTexture(GL_COLOR_ATTACHMENT2, norm_depthTexture);
		_renderer.renderToFrameBuferObject(fbo);

		auto program = new Program;
		program->attachShader(GL_VERTEX_SHADER, "datas/SSAO/DeferredShader.vert");
		program->attachShader(GL_FRAGMENT_SHADER, "datas/SSAO/DeferredShader.frag");
		model->setProgram(program);
		_renderer.attachRenderableObject(model);

		_rttTexture = SSAO(posTexture, norm_depthTexture, 0.02f, 2.0f, size);
		auto blurTex = GaussBlur(_rttTexture, size, 4);

		//////////////////////////////////////////////////////////////////////////
		static const GLchar *V_ShaderSource[] = {
			"#version 430                        \n"
			"layout(location = 0) in vec4 position;                       \n"
			"layout(location = 1) in vec2 uv;                       \n"
			"uniform mat4 u_ModelViewProjectMat;                                      \n"
			"out vec2 v_uv;                                          \n"
			"void main(){                        \n"
			"	v_uv = uv;                                \n"
			"	gl_Position = u_ModelViewProjectMat * position;                        \n"
			"}                        \n"
		};

		static const GLchar *F_ShaderSource[] = {
			"#version 430                        \n"
			"uniform sampler2D u_Texture0;                       \n"
			"uniform sampler2D u_Texture1;                       \n"
			"in vec2 v_uv;                                          \n"
			"out vec4 color;                        \n"
			"void main(){                        \n"
			"	color = (texture(u_Texture0, v_uv) + texture(u_Texture1, v_uv)) * 0.5;     \n"
			"}                        \n"
		};

		auto rttRender = new Renderer();
		rttRender->setClearColor(glm::vec4(1.0f));
		rttRender->depthTest(false);
		RenderQuad *rq = new RenderQuad(size.x, size.y);
		program = new Program;
		program->attachShader(GL_VERTEX_SHADER, V_ShaderSource);
		program->attachShader(GL_FRAGMENT_SHADER, F_ShaderSource);
		rq->setProgram(program);
		rq->setTexture(0, blurTex, TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP));
		rq->setTexture(1, colorTexture, TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP));
		rttRender->attachRenderableObject(rq);
		_rttRenders.push_back(rttRender);
	}

	GLuint GaussBlur(GLuint inTexture, glm::ivec2 size, float sgm2 = 36){

		//////////////////////////////////////////////////////////////////////////
		static const unsigned int sampRadius = 5;
		float weights[sampRadius], sum, sigma2 = sgm2;
		weights[0] = gaussFunc(0, sigma2);
		sum = weights[0];
		for (int i = 1; i < sampRadius; ++i){
			weights[i] = gaussFunc(i, sigma2);
			sum += 2 * weights[i];
		}

		for (int i = 0; i < sampRadius; ++i){
			weights[i] /= sum;
		}
		//////////////////////////////////////////////////////////////////////////

		/*height blur*/
		//////////////////////////////////////////////////////////////////////////
		auto heightBlurTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		{
			auto rttRender = new Renderer();
			rttRender->setClearColor(glm::vec4(1.0f));
			rttRender->depthTest(false);
			auto fbo = new FrameBufferObject;
			fbo->attachTexture(GL_COLOR_ATTACHMENT0, heightBlurTexture);
			rttRender->renderToFrameBuferObject(fbo);

			static const GLchar *vShaderSource[] = {
				"#version 430                        \n"
				"layout(location = 0) in vec4 position;                       \n"
				"layout(location = 1) in vec2 uv;                       \n"
				"uniform mat4 u_ModelViewProjectMat;                                      \n"
				"out vec2 v_uv;                                          \n"
				"void main(){                        \n"
				"	v_uv = uv;                                \n"
				"	gl_Position = u_ModelViewProjectMat * position;                        \n"
				"}                        \n"
			};

			static const GLchar *fShaderSource[] = {
				"#version 430                        \n"
				"uniform sampler2D u_Texture0;                       \n"
				"uniform float weights[5];                     \n"
				"uniform float Height;                  \n"
				"in vec2 v_uv;                                          \n"
				"out vec4 fragColor;                        \n"
				"void main(){                        \n"
				"	float dy = 1.0 / Height;                        \n"
				"	vec4 sum = texture(u_Texture0, v_uv) * weights[0];                  \n"
				"	for (int i = 1; i < 5; ++i){                          \n"
				"		sum += texture(u_Texture0, v_uv + vec2(0.0, i * dy)) * weights[i];          \n"
				"		sum += texture(u_Texture0, v_uv - vec2(0.0, i * dy)) * weights[i];          \n"
				"	}          \n"
				"	fragColor = sum;           \n"
				"}                        \n"
			};

			RenderQuad *rq = new RenderQuad(size.x, size.y);
			auto program = new Program;
			program->attachShader(GL_VERTEX_SHADER, vShaderSource);
			program->attachShader(GL_FRAGMENT_SHADER, fShaderSource);
			program->setUniformf("Height", size.y);
			for (unsigned int i = 0; i < sampRadius; ++i){
				char buf[20];
				sprintf(buf, "weights[%d]", i);
				program->setUniformf(buf, weights[i]);
			}
			rq->setProgram(program);
			auto sampler = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP);
			rq->setTexture(0, inTexture, sampler);
			rttRender->attachRenderableObject(rq);
			_rttRenders.push_back(rttRender);
		}
		//////////////////////////////////////////////////////////////////////////

		/*width blur*/
		//////////////////////////////////////////////////////////////////////////
		auto outTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		{
			auto rttRender = new Renderer();
			rttRender->setClearColor(glm::vec4(1.0f));
			rttRender->depthTest(false);
			auto fbo = new FrameBufferObject;
			fbo->attachTexture(GL_COLOR_ATTACHMENT0, outTexture);
			rttRender->renderToFrameBuferObject(fbo);

			static const GLchar *vShaderSource[] = {
				"#version 430                        \n"
				"layout(location = 0) in vec4 position;                       \n"
				"layout(location = 1) in vec2 uv;                       \n"
				"uniform mat4 u_ModelViewProjectMat;                                      \n"
				"out vec2 v_uv;                                          \n"
				"void main(){                        \n"
				"	v_uv = uv;                                \n"
				"	gl_Position = u_ModelViewProjectMat * position;                        \n"
				"}                        \n"
			};

			static const GLchar *fShaderSource[] = {
				"#version 430                        \n"
				"uniform sampler2D u_Texture0;                       \n"
				"uniform float weights[5];                     \n"
				"uniform float Width;                  \n"
				"in vec2 v_uv;                                          \n"
				"out vec4 fragColor;                        \n"
				"void main(){                        \n"
				"	float dx = 1.0 / Width;                        \n"
				"	vec4 sum = texture(u_Texture0, v_uv) * weights[0];                  \n"
				"	for (int i = 1; i < 5; ++i){                          \n"
				"		sum += texture(u_Texture0, v_uv + vec2(i * dx, 0.0)) * weights[i];          \n"
				"		sum += texture(u_Texture0, v_uv - vec2(i * dx, 0.0)) * weights[i];          \n"
				"	}          \n"
				"	fragColor = sum;           \n"
				"}                        \n"
			};

			RenderQuad *rq = new RenderQuad(size.x, size.y);
			auto program = new Program;
			program->attachShader(GL_VERTEX_SHADER, vShaderSource);
			program->attachShader(GL_FRAGMENT_SHADER, fShaderSource);
			program->setUniformf("Width", size.x);
			for (unsigned int i = 0; i < sampRadius; ++i){
				char buf[20];
				sprintf(buf, "weights[%d]", i);
				program->setUniformf(buf, weights[i]);
			}
			rq->setProgram(program);
			auto sampler = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP);
			rq->setTexture(0, heightBlurTexture, sampler);
			rttRender->attachRenderableObject(rq);
			_rttRenders.push_back(rttRender);
		}
		//////////////////////////////////////////////////////////////////////////
		return outTexture;
	}

	GLuint ExtractBright(GLuint inTexture, float LumThresh, glm::ivec2 size){
		auto outTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		{
			auto rttRender = new Renderer();
			rttRender->setClearColor(glm::vec4(1.0f));
			rttRender->depthTest(false);
			auto fbo = new FrameBufferObject;
			fbo->attachTexture(GL_COLOR_ATTACHMENT0, outTexture);
			rttRender->renderToFrameBuferObject(fbo);

			static const GLchar *vShaderSource[] = {
				"#version 430                        \n"
				"layout(location = 0) in vec4 position;                       \n"
				"layout(location = 1) in vec2 uv;                       \n"
				"uniform mat4 u_ModelViewProjectMat;                                      \n"
				"out vec2 v_uv;                                          \n"
				"void main(){                        \n"
				"	v_uv = uv;                                \n"
				"	gl_Position = u_ModelViewProjectMat * position;                        \n"
				"}                        \n"
			};

			static const GLchar *fShaderSource[] = {
				"#version 430                        \n"
				"uniform sampler2D u_Texture0;                       \n"
				"uniform float LumThresh;                     \n"
				"in vec2 v_uv;                                          \n"
				"out vec4 fragColor;                        \n"
				"float luma( vec3 color ) {                     \n"
				"	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;                     \n"
				"}                     \n"
				"void main(){                        \n"
				"	vec4 color = texture(u_Texture0, v_uv);                  \n"
				"	fragColor = color * clamp(luma(color.rgb) - LumThresh, 0.0, 1.0)*(1.0 / (1.0 - LumThresh));                     \n"
				"}                        \n"
			};

			RenderQuad *rq = new RenderQuad(size.x, size.y);
			auto program = new Program;
			program->attachShader(GL_VERTEX_SHADER, vShaderSource);
			program->attachShader(GL_FRAGMENT_SHADER, fShaderSource);
			program->setUniformf("LumThresh", LumThresh);
			rq->setProgram(program);
			auto sampler = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP);
			rq->setTexture(0, inTexture, sampler);
			rttRender->attachRenderableObject(rq);
			_rttRenders.push_back(rttRender);
		}

		return outTexture;
	}

	GLuint SSAO(GLuint posTexture, GLuint normalTexture, float aoDensity, float contrasty, glm::ivec2 size){
		GLuint offsetSampU = 4;
		GLuint offsetSampV = 4;
		auto offsetTex = CreateRandomPointsImageInSphereSurface(offsetSampU, offsetSampV);
		auto outTexture = TextureManager::Instance()->CreateTexture(size.x, size.y, 1, GL_RGBA32F);
		{
			auto rttRender = new Renderer();
			rttRender->setClearColor(glm::vec4(1.0f));
			rttRender->depthTest(false);
			auto fbo = new FrameBufferObject;
			fbo->attachTexture(GL_COLOR_ATTACHMENT0, outTexture);
			rttRender->renderToFrameBuferObject(fbo);

			RenderQuad *rq = new RenderQuad(size.x, size.y);
			auto program = new Program;
			program->attachShader(GL_VERTEX_SHADER, "datas/SSAO/SSAOShader.vert");
			program->attachShader(GL_FRAGMENT_SHADER, "datas/SSAO/SSAOShader.frag");
			program->setUniformf("aoDensity", aoDensity);
			program->setUniformf("contrasty", contrasty);
			program->setUniform3f("OffsetTexSize", glm::vec3(offsetSampU, offsetSampV, offsetSampU * offsetSampV));
			rq->setProgram(program);
			auto sampler = TextureManager::Instance()->GetOrCreateSampler(GL_LINEAR, GL_CLAMP);
			rq->setTexture(0, posTexture, sampler);
			rq->setTexture(1, normalTexture, sampler);
			rq->setTexture(2, offsetTex, sampler);
			rttRender->attachRenderableObject(rq);
			_rttRenders.push_back(rttRender);
		}

		return outTexture;
	}

private:

	std::vector<Renderer*> _rttRenders;
	GLuint _rttTexture;
};