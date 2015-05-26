#include "Node.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

Node::Node()
	: _parent(nullptr)
	, _rObject(nullptr)
	, _boundingSphere(new BoundingSphere(glm::vec3(), 0.0f))
	, _boundingBox(new BoundingBox)
	, _dirtyBS(true)
	, _dirtyBB(true)
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
	dirty();
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
	dirty();
}

bool Node::removeChild( unsigned int pos )
{
	if (_children.size() <= pos) return false;
	_children[pos]->_parent = nullptr;
	_children.erase(_children.begin() + pos);
	dirty();
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

BoundingSphere* Node::getBoundingSphere()
{
	if (_dirtyBS){
		glm::vec3 center;
		float radius = 0.0;
		if (_rObject && _rObject->getBoundingSphere()){
			center = _rObject->getBoundingSphere()->getCenter();
			radius = _rObject->getBoundingSphere()->getRadius();
		}

		if (!_children.empty()){
			for (const auto &iter : _children){
				center += iter->getBoundingSphere()->getCenter();
			}
			center = center / (float)_children.size();
			for (const auto &iter : _children){
				float dis = glm::length(iter->getBoundingSphere()->getCenter() - center) + iter->getBoundingSphere()->getRadius();
				radius = radius < dis? dis: radius;
			}
		}

		_boundingSphere->setCenter((_matrix * glm::vec4(center, 1.0f)).xyz);
		_boundingSphere->setRadius(radius);
		_dirtyBS = false;
	}
	return _boundingSphere;
}

BoundingBox* Node::getBoundingBox()
{
	if (_dirtyBB){
		_boundingBox->reset();
		if (_rObject && _rObject->getBoundingBox()){
			_boundingBox->setMin(_rObject->getBoundingBox()->getMin());
			_boundingBox->setMax(_rObject->getBoundingBox()->getMax());
		}

		if (!_children.empty()){
			for (const auto &iter : _children){
				expandBoundingBoxBy(iter->getBoundingBox()->getMin(), iter->getBoundingBox()->getMax());
			}
		}

		_dirtyBB = false;
	}

	return _boundingBox;
}

void Node::dirty()
{
	_dirtyBS = _dirtyBB = true;
	if (_parent)
		_parent->dirty();
}

void Node::setMatrix( const glm::mat4 &mat )
{
	_matrix = mat;
	dirty();
}

void Node::expandBoundingBoxBy( const glm::vec3 &min, const glm::vec3 &max )
{
	_boundingBox->expandBy((_matrix * glm::vec4(min.x, min.y, min.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(max.x, min.y, min.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(max.x, max.y, min.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(min.x, max.y, min.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(min.x, min.y, max.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(max.x, min.y, max.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(max.x, max.y, max.z, 1.0f)).xyz);
	_boundingBox->expandBy((_matrix * glm::vec4(min.x, max.y, max.z, 1.0f)).xyz);
}
