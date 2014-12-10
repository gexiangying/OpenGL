#include "ApplicationFactory.h"
#include "TransformFeedbackApp.h"
#include "TriangleApp.h"
#include "ModelImporterApp.h"
#include "AdvanceLightApp.h"

ApplicationFactory::ApplicationFactory()
{
}


ApplicationFactory::~ApplicationFactory()
{
}

ApplicationFactory* ApplicationFactory::Instance()
{
	static ApplicationFactory af;
	return &af;
}

Application* ApplicationFactory::CreateApplication( const std::string &type )
{
	if (type == "TransformFeedbackApp")
		return new TransformFeedbackApp;

	if (type == "TriangleApp")
		return new TriangleApp;

	if (type == "ModelImporterApp")
		return new ModelImporterApp;

	if (type == "AdvanceLightApp")
		return new AdvanceLightApp;

	return nullptr;
}
