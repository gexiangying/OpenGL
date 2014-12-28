#pragma once
#include "Mesh.h"
#include "RenderableObject.h"

struct aiNode;
struct aiScene;
class MoveableNode : public RenderableObject
{
public:
	typedef std::vector<MoveableNode *> MoveableNodeList;
	typedef std::vector<Mesh *> MeshList;

	MoveableNode(aiNode *node, const aiScene *scene, const std::string &path);
	~MoveableNode();

	void setProgram(Program *program, bool needOverride = false);

	virtual void draw(const Renderer *renderer) override;

	void addMesh(Mesh *subMesh);
	unsigned int getMeshsCount() const;
	Mesh* getMesh(unsigned int index);

	void addChild(MoveableNode *node);
	unsigned int getChildrenCount() const;
	MoveableNode* getChild(unsigned int index);

private:

	void buildMeshs(const std::string &path);
	void buildChildren(const std::string &path);

private:

	MoveableNodeList _children;
	MeshList _subMesh;
	aiNode *_node;
	const aiScene *_rootScene;
	glm::mat4 _matrix;
};

