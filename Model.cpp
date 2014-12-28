#include "Model.h"
#include "Mesh.h"
#include "Renderer.h"


Model::Model(const std::string &file)
	: _filePath(file)
	, _aiScene(nullptr)
	, _rootNode(nullptr)
{
	decomposeFilePath();
	initLogs();
	loadScene();
}


Model::~Model()
{
	aiReleaseImport(_aiScene);
	aiDetachAllLogStreams();
	delete _rootNode;
}

void Model::draw(const Renderer *renderer)
{
	_rootNode->draw(renderer);
}

void Model::initLogs()
{
	aiLogStream stream;

	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);
}

void Model::loadScene()
{
	_aiScene = aiImportFile(_filePath.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	_rootNode = new MoveableNode(_aiScene->mRootNode, _aiScene, _folderPath);
}

void Model::setProgram( Program *program, bool needOverride /*= false*/ )
{
	_rootNode->setProgram(program, needOverride);
}

void Model::decomposeFilePath()
{
	std::string::size_type pos = _filePath.find_last_of("/\\");
	if (pos != std::string::npos){
		_folderPath = _filePath.substr(0, pos + 1);
		_fileName = _filePath.substr(pos + 1);
	}
}
