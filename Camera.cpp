#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::lookAt( const glm::vec3 &eye, const glm::vec3 &center, glm::vec3 &up )
{
	_viewMat = glm::lookAt(eye, center, up);
}

void Camera::setProjectionMatrixAsPerspective( float fovy, float aspectRatio, float zNear, float zFar )
{
	_projectMat = glm::perspective(fovy, aspectRatio, zNear, zFar);
}

void Camera::setProjectionMatrixAsOrtho( float left, float right, float bottom, float top, float zNear, float zFar )
{
	_projectMat = glm::ortho(left, right, bottom, top, zNear, zFar);
}
