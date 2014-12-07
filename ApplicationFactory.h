#pragma once
#include "Application.h"
#include <string>

class ApplicationFactory
{
public:

	static ApplicationFactory* Instance();
	Application* CreateApplication(const std::string &type);

private:
	ApplicationFactory();
	~ApplicationFactory();
};

