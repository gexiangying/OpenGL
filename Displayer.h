#pragma once
#include <glm/glm.hpp>
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

private:

	Displayer();
	~Displayer();

private:

	glm::ivec2 _winSize;
	std::string _title;
	unsigned int _displayMode;
};

