#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Displayer
{
public:

	static Displayer* Instance();
	const glm::ivec2& GetWindowSize();
	void SetWindowSize(const glm::ivec2 &size);
	void SetWindowTitle(const std::string& title);
	const std::string& GetWindowTitle();
	void SetDisplayMode(unsigned int mode);
	unsigned int GetDisplayMode();
	void setProjectionMatrix(const glm::mat4 &mat){ _projectionMat = mat; };
	const glm::mat4& getProjectionMatrix()const { return _projectionMat; };

private:

	Displayer();
	~Displayer();

private:

	glm::mat4 _projectionMat;
	glm::ivec2 _winSize;
	std::string _title;
	unsigned int _displayMode;
};

