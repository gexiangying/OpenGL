#pragma once
#include <string>

class Node;
class Geometry;
struct aiNode;
struct aiMesh;
struct aiScene;
class AssimpConverter
{
public:

	~AssimpConverter();

	static AssimpConverter* Instance();

	Node* LoadModelFile(const std::string &filePath);

protected:
	AssimpConverter();

	void convertaiNodeToNode(aiNode *ainode, Node *node);
	void convertaiMeshToGeometry(aiMesh *aimesh, Geometry *geometry);

private:

	const aiScene *_scene;
	std::string _currentFolder;
};

