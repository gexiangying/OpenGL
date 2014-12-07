#include "Displayer.h"
#include <GL/glut.h>

Displayer::Displayer()
	: _winSize(800, 600)
	, _title("Application")
	, _displayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
{
}


Displayer::~Displayer()
{
}

Displayer* Displayer::Instance()
{
	static Displayer dis;
	return &dis;
}

const glm::ivec2& Displayer::GetWindowSize()
{
	return _winSize;
}

void Displayer::SetWindowSize( const glm::ivec2 &size )
{
	_winSize = size;
}

void Displayer::SetDisplayMode( unsigned int mode )
{
	_displayMode = mode;
}

unsigned int Displayer::GetDisplayMode()
{
	return _displayMode;
}

void Displayer::SetWindowTitle( const std::string& title )
{
	_title = title;
}

const std::string& Displayer::GetWindowTitle()
{
	return _title;
}
