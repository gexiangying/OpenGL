#pragma once
#include <string>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include "MoveableNode.h"

class MoveableNode;
class Model
{
public:
	
	Model(const std::string &file);
	~Model();

	void draw(const glm::mat4x4 &modelView);

private:

	void initLogs();
	void loadScene();

private:
	std::string _filePath;
	const aiScene *_aiScene;
	MoveableNode *_rootNode;
};

