#pragma once
#include "Mesh.h"

struct aiNode;
struct aiScene;
class MoveableNode
{
public:
	typedef std::vector<MoveableNode *> MoveableNodeList;
	typedef std::vector<Mesh *> MeshList;

	MoveableNode(aiNode *node, const aiScene *scene);
	~MoveableNode();

	void draw(const glm::mat4x4 &modelView);

	void addMesh(Mesh *subMesh);
	unsigned int getMeshsCount() const;
	Mesh* getMesh(unsigned int index);

	void addChild(MoveableNode *node);
	unsigned int getChildrenCount() const;
	MoveableNode* getChild(unsigned int index);

private:

	void buildMeshs();
	void buildChildren();

private:

	MoveableNodeList _children;
	MeshList _subMesh;
	aiNode *_node;
	const aiScene *_rootScene;
	glm::mat4 _matrix;
};

