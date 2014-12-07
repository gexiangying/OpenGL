#include "Application.h"
#include "Displayer.h"
#include <iostream>

Application * Application::_appPtr;

Application::Application()
{
	_appPtr = this;
}


Application::~Application()
{
}

void Application::Setup()
{

}

void Application::Render(double elapseTime)
{
	std::cout<<"Render"<<std::endl;
}

void Application::Run()
{
	_preFrameTime = std::time(nullptr);
	//while (_running)
	//{
	//	std::time_t now = std::time(nullptr);
	//	Render();
	//	_preFrameTime = now;
	//	glutSwapBuffers();
	//}
	glutMainLoop();
}

void Application::Init(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(Displayer::Instance()->GetDisplayMode());
	glutInitWindowSize(Displayer::Instance()->GetWindowSize().x, Displayer::Instance()->GetWindowSize().y);
	glutCreateWindow(Displayer::Instance()->GetWindowTitle().c_str());
	
	glutDisplayFunc(Application::renderStatic);
	glewInit();
	Setup();
	printGLInfo();
}

void Application::printGLInfo()
{
	const GLubyte* byteGlVersion = glGetString(GL_VERSION);  
	const GLubyte* byteGlVendor = glGetString(GL_VENDOR);  
	const GLubyte* byteGlRenderer = glGetString(GL_RENDERER);  
	const GLubyte* byteSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);  

	std::cout<<"--------------------------- OpenGL Info ---------------------------"<<std::endl;
	std::cout<<"OpenGL version: %s\n"<<byteGlVersion<<std::endl;
	std::cout<<"OpenGL Vender: %s\n"<<byteGlVendor<<std::endl;
	std::cout<<"OpenGL Renerer: %s\n"<<byteGlRenderer<<std::endl;
	std::cout<<"GLSL version: %s\n"<<byteSLVersion<<std::endl;
	std::cout<<"--------------------------- OpenGL Info ---------------------------"<<std::endl;
}

void Application::Stop()
{
}

void Application::renderStatic()
{
	std::time_t now = std::time(nullptr);
	_appPtr->Render(std::difftime(now, _appPtr->_preFrameTime));
	_appPtr->_preFrameTime = now;
	glutSwapBuffers();
	glutPostRedisplay();
}

bool Application::CompileShader(GLuint shader){
	glCompileShader(shader);
	GLint state; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
	if (true){
		GLint maxLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
		if (maxLen > 0){
			GLchar *errors = new GLchar[maxLen];
			glGetShaderInfoLog(shader, maxLen, &maxLen, errors);
			printf("%s", errors);
			delete[] errors;
		}
	}
	return state == GL_TRUE? true: false;
}
