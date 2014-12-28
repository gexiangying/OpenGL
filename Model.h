#pragma once
#include <string>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include "MoveableNode.h"
#include "RenderableObject.h"

class MoveableNode;
class Renderer;
class Model : public RenderableObject
{
public:
	
	Model(const std::string &file);
	~Model();

	void setProgram(Program *program, bool needOverride = false);

	virtual void draw(const Renderer *renderer) override;

private:

	void initLogs();
	void loadScene();
	void decomposeFilePath();

private:
	std::string _filePath;
	std::string _fileName;
	std::string _folderPath;
	const aiScene *_aiScene;
	MoveableNode *_rootNode;
};

