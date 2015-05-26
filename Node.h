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
	virtual BoundingSphere* getBoundingSphere() override;
	virtual BoundingBox* getBoundingBox() override;

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
	void setMatrix(const glm::mat4 &mat);
	const glm::mat4& getMatrix() const { return _matrix; }
	void dirty();

protected:

	void expandBoundingBoxBy(const glm::vec3 &min, const glm::vec3 &max);

protected:

	Node *_parent;
	NodeList _children;
	glm::mat4 _matrix;
	RenderableObject *_rObject;
	BoundingSphere *_boundingSphere;
	BoundingBox *_boundingBox;
	bool _dirtyBS;
	bool _dirtyBB;
};

