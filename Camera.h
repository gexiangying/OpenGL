#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void lookAt(const glm::vec3 &eye, const glm::vec3 &center, glm::vec3 &up);
	void setViewMatrix(const glm::mat4 &mat) { _viewMat = mat; };
	const glm::mat4& getViewMatrix() const { return _viewMat; };

	void setProjectionMatrixAsOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void setProjectionMatrixAsPerspective(float fovy, float aspectRatio, float zNear, float zFar);
	void setProjectionMatrix(const glm::mat4 &mat) { _projectMat = mat; };
	const glm::mat4& getProjectionMatrix() const { return _projectMat; }

private:

	glm::mat4 _viewMat;
	glm::mat4 _projectMat;
};

