#include "ApplicationFactory.h"

int main(int argc, char *argv[]){

	std::string apps[] = {
		"TriangleApp",
		"TransformFeedbackApp",
		"ModelImporterApp",
		"AdvanceLightApp"
	};
	Application *app = ApplicationFactory::Instance()->CreateApplication(apps[3]);

	app->Init(argc, argv);
	app->Run();
	return 0;
}