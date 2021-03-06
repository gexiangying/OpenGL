#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ctime>
#include "Camera.h"
#include "Renderer.h"

class Displayer;
class Application
{
public:
	Application();
	virtual ~Application();

	void Init(int argc, char *argv[]);
	void Run();
	void Stop();

protected:

	virtual void Setup() = 0;
	virtual void Render(double elapseTime) = 0;
	virtual void mouseClickEvent(int button, int state, int x, int y){};
	virtual void mouseMoveEvent(int x, int y){};
	virtual void keyboardEvent(unsigned char key, int x, int y){};
	virtual void specialKeyEvent(int key, int x, int y){};
	void printGLInfo();
	bool CompileShader(GLuint shader);

	void initCamera();
	void rotateCamera(const glm::vec2 &g0, const glm::vec2 &g1);
	void moveCamera(const glm::vec2 &g0, const glm::vec2 &g1);
	void zoomCamera(const glm::vec2 &g0, const glm::vec2 &g1);
	void updateViewMatrix();

	void trackball( glm::vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y );
	float tb_project_to_sphere( float r, float x, float y );

	static void renderStatic();
	static void mouseClickEventStatic(int button, int state, int x, int y);
	static void mouseMoveEventStatic(int x, int y);
	static void keyboardEventStatic(unsigned char key, int x, int y);
	static void specialKeyEventStatic(int key, int x, int y);
	static glm::vec2 convertScreenCoordsToGLCoords(int x, int y);

protected:

	static Application *_appPtr;

	std::time_t _preFrameTime;

	int _currentButton;

	glm::vec2 _g0;
	glm::vec2 _g1;

	glm::quat _rotation;
	glm::vec3 _center;
	glm::vec3 _translate;

	Camera *_camera;
	Renderer _renderer;

	bool _running;
};

