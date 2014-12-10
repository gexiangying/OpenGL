#include "MoveableNode.h"
#include <assimp/scene.h>


MoveableNode::MoveableNode(aiNode *node, const aiScene *scene)
	: _node(node)
	, _rootScene(scene)
{
	aiMatrix4x4 mat = _node->mTransformation;
	_matrix = glm::mat4(mat.a1, mat.a2, mat.a3, mat.a3
					  , mat.b1, mat.b2, mat.b3, mat.b4
					  , mat.c1, mat.c2, mat.c3, mat.c4
					  , mat.d1, mat.d2, mat.d3, mat.d4);

	buildMeshs();
	buildChildren();
}


MoveableNode::~MoveableNode()
{
	for (auto iter : _subMesh){
		delete iter;
	}

	for (auto iter : _children){
		delete iter;
	}
}

void MoveableNode::addMesh( Mesh *subMesh )
{
	_subMesh.push_back(subMesh);
}

unsigned int MoveableNode::getMeshsCount() const
{
	return _subMesh.size();
}

Mesh* MoveableNode::getMesh( unsigned int index )
{
	return _subMesh[index];
}

void MoveableNode::draw( const glm::mat4x4 &modelView )
{
	for (auto iter : _children){
		iter->draw(modelView * _matrix);
	}

	for (auto iter : _subMesh){
		iter->draw(modelView * _matrix);
	}
}

void MoveableNode::buildMeshs()
{
	for (unsigned int i = 0; i < _node->mNumMeshes; ++i){
		aiMesh *mesh = _rootScene->mMeshes[_node->mMeshes[i]];
		aiMaterial *material = _rootScene->mMaterials[mesh->mMaterialIndex];
		Mesh *renderMesh = new Mesh(mesh, material);
		addMesh(renderMesh);
	}
}

void MoveableNode::addChild( MoveableNode *node )
{
	_children.push_back(node);
}

unsigned int MoveableNode::getChildrenCount() const
{
	return _children.size();
}

MoveableNode* MoveableNode::getChild( unsigned int index )
{
	return _children[index];
}

void MoveableNode::buildChildren()
{
	for (unsigned int i = 0; i < _node->mNumChildren; ++i){
		MoveableNode *node = new MoveableNode(_node->mChildren[i], _rootScene);
		addChild(node);
	}
}

void MoveableNode::setProgram( Program *program, bool needOverride /*= false*/ )
{
	for (auto iter : _children){
		iter->setProgram(program, needOverride);
	}

	for (auto iter : _subMesh){
		iter->setProgram(program);
	}
}
