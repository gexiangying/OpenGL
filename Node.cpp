#include "Node.h"


Node::Node()
	: _parent(nullptr)
	, _rObject(nullptr)
{
}


Node::~Node()
{
}

void Node::draw( const Renderer *renderer, const glm::mat4 &modelMatrix )
{
	for (auto iter : _children){
		iter->draw(renderer, modelMatrix * _matrix);
	}

	if (_rObject)
		_rObject->draw(renderer, modelMatrix * _matrix);
}

bool Node::addChild( Node *node )
{
	auto iter = std::find(_children.begin(), _children.end(), node);
	if (iter != _children.end()) return false;
	node->_parent = this;
	_children.push_back(node);
	return true;
}

Node* Node::getChild( unsigned int pos )
{
	if (_children.size() <= pos) return nullptr;
	return _children[pos];
}

void Node::attachRenderableObject( RenderableObject *object )
{
	_rObject = object;
}

bool Node::removeChild( unsigned int pos )
{
	if (_children.size() <= pos) return false;
	_children[pos]->_parent = nullptr;
	_children.erase(_children.begin() + pos);
	return true;
}

bool Node::removeChild( Node *node )
{
	auto iter = std::find(_children.begin(), _children.end(), node);
	if (iter != _children.end()){
		removeChild(iter - _children.begin());
		return true;
	}
	return false;
}

glm::mat4 Node::getNodeToWorldMatrix() const
{
	glm::mat4 mat = _matrix;
	if (_parent){
		mat = _parent->getNodeToWorldMatrix() * mat;
	}
	return mat;
}

glm::mat4 Node::getWorldToNodeMatrix() const
{
	glm::mat4 mat = _matrix;
	if (_parent){
		mat = mat * _parent->getWorldToNodeMatrix();
	}
	return mat;
}
