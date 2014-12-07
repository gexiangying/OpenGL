#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <ctime>

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
	void printGLInfo();
	bool CompileShader(GLuint shader);
	static void renderStatic();

protected:
	std::time_t _preFrameTime;
	static Application *_appPtr;
};

