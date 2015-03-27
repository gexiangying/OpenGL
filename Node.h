#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "RenderableObject.h"

class Node;
typedef std::vector<Node *> NodeList;
class Node : public RenderableObject
{
public:
	Node();
	~Node();

	virtual void draw(const Renderer *renderer, const glm::mat4 &modelMatrix) override;

	bool addChild(Node *node);
	Node* getChild(unsigned int pos);
	bool removeChild(unsigned int pos);
	bool removeChild(Node *node);
	unsigned int getNumChildren() const { return _children.size(); };
	Node* getParent() const { return _parent; }
	void attachRenderableObject(RenderableObject *object);
	RenderableObject* getAttachedRenderableObject() const { return _rObject; }

	glm::mat4 getNodeToWorldMatrix() const;
	glm::mat4 getWorldToNodeMatrix() const;
	void setMatrix(const glm::mat4 &mat) { _matrix = mat; }
	const glm::mat4& getMatrix() const { return _matrix; }

protected:

	Node *_parent;
	NodeList _children;
	glm::mat4 _matrix;
	RenderableObject *_rObject;
};

