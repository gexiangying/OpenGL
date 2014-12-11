#include "Application.h"
#include "Displayer.h"
#include <iostream>

#define GLUT_WHEEL_UP 0x0003
#define GLUT_WHEEL_DOWN 0x0004

Application * Application::_appPtr;

Application::Application()
	: _currentButton(-1)
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
	glutMouseFunc(Application::mouseClickEventStatic);
	glutMotionFunc(Application::mouseMoveEventStatic);
	glutKeyboardFunc(Application::keyboardEventStatic);
	glutSpecialFunc(Application::specialKeyEventStatic);
	glewInit();
	initCamera();
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
	std::cout<<"OpenGL version: "<<byteGlVersion<<std::endl;
	std::cout<<"OpenGL Vender: "<<byteGlVendor<<std::endl;
	std::cout<<"OpenGL Renerer: "<<byteGlRenderer<<std::endl;
	std::cout<<"GLSL version: "<<byteSLVersion<<std::endl;
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

void Application::mouseClickEventStatic( int button, int state, int x, int y )
{
	_appPtr->_g0 = convertScreenCoordsToGLCoords(x, y);

	if (state == GLUT_DOWN){
		_appPtr->_currentButton = button;
	}

	if (state == GLUT_UP){
		_appPtr->_currentButton = -1;
	}

	if (button == GLUT_WHEEL_DOWN){
		_appPtr->zoomCamera(glm::vec2(0.1f), glm::vec2(0.0f));
	}

	if (button == GLUT_WHEEL_UP){
		_appPtr->zoomCamera(glm::vec2(0.0f), glm::vec2(0.1f));
	}

	_appPtr->mouseClickEvent(button, state, x, y);
}

void Application::mouseMoveEventStatic( int x, int y )
{
	_appPtr->_g1 = convertScreenCoordsToGLCoords(x, y);
	//std::cout<<"g1 - g0: "<<_appPtr->_g1.x - _appPtr->_g0.x<<", "<<_appPtr->_g1.y - _appPtr->_g0.y<<std::endl;

	//std::cout<<"DOWN MOVE"<<x<<", "<<y<<std::endl;
	if (_appPtr->_currentButton == GLUT_LEFT_BUTTON){
		_appPtr->rotateCamera(_appPtr->_g0, _appPtr->_g1);
	}

	if (_appPtr->_currentButton == GLUT_MIDDLE_BUTTON){
		_appPtr->moveCamera(_appPtr->_g0, _appPtr->_g1);
	}

	if (_appPtr->_currentButton == GLUT_RIGHT_BUTTON){
		_appPtr->zoomCamera(_appPtr->_g0, _appPtr->_g1);
	}

	_appPtr->mouseMoveEvent(x, y);

	_appPtr->_g0 = _appPtr->_g1;
}

glm::vec2 Application::convertScreenCoordsToGLCoords( int x, int y )
{
	const glm::ivec2 &size = Displayer::Instance()->GetWindowSize();	
	return glm::vec2(2.0f * (float)x / (float)size.x - 1.0f, 2.0f * (float)(size.y - y) / (float)size.y - 1.0f);
}

void Application::trackball( glm::vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y )
{
	float trackballSize = 1.0f;

	glm::vec3 uv = _rotation * glm::vec3(0.0f,1.0f,0.0f);
	glm::vec3 sv = _rotation * glm::vec3(1.0f,0.0f,0.0f);
	glm::vec3 lv = _rotation * glm::vec3(0.0f,0.0f,-1.0f);

	glm::vec3 p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(trackballSize, p1x, p1y);
	glm::vec3 p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(trackballSize, p2x, p2y);

	axis = glm::cross(p2, p1);
	axis = glm::normalize(axis);

	float t = (p2 - p1).length() / (2.0f * trackballSize);

	if (t > 1.0f) t = 1.0f;
	if (t < -1.0f) t = -1.0f;
	angle = asin(t);
}

float Application::tb_project_to_sphere( float r, float x, float y )
{
	float d, t, z;
	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440f)
	{
		z = sqrt(r*r - d*d);
	}                         
	else
	{
		t = r / 1.41421356237309504880f;
		z = t*t / d;
	}
	return z;
}

void Application::rotateCamera( const glm::vec2 &g0, const glm::vec2 &g1 )
{
	if (g0 == g1) return;

	float angle;
	glm::vec3 axes;
	trackball(axes, angle, g0.x, g0.y, g1.x, g1.y);
	_rotation = glm::angleAxis(angle * 3.0f, axes) * _rotation;
	updateMVPMatrix();
}

void Application::moveCamera( const glm::vec2 &g0, const glm::vec2 &g1 )
{
	if (g0 == g1) return;
	glm::vec2 delta = g0 - g1;
	delta *= 20.0f;
	_center += _rotation * glm::vec3(delta, 0.0f);
	updateMVPMatrix();
}

void Application::initCamera()
{
	_center = glm::vec3(0.0f);
	_translate = glm::vec3(0.0f, 0.0f, 150.0f);
	Displayer::Instance()->setProjectionMatrix(glm::perspective(30.0f, (float)Displayer::Instance()->GetWindowSize().x / (float)Displayer::Instance()->GetWindowSize().y, 1.0f, 1000.0f));
	updateMVPMatrix();
}

void Application::updateMVPMatrix()
{
	glm::mat4 center;
	center[3] = glm::vec4(_center, 1.0f);
	glm::mat4 trans;
	trans[3] = glm::vec4(_translate, 1.0f);

	_viewMat = glm::inverse(center * glm::toMat4(_rotation) * trans);
}

void Application::zoomCamera( const glm::vec2 &g0, const glm::vec2 &g1 )
{
	if (g0 == g1) return;
	float zoomDleta = g0.y - g1.y;
	_translate.z += zoomDleta * 20.0f;
	updateMVPMatrix();
}

void Application::keyboardEventStatic( unsigned char key, int x, int y )
{
	_appPtr->keyboardEvent(key, x, y);
}

void Application::specialKeyEventStatic( int key, int x, int y )
{
	_appPtr->specialKeyEvent(key, x, y);
}
