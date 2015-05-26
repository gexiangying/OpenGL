#include "AssimpConverter.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Node.h"
#include "Geometry.h"
#include "Material.h"
#include "TextureManager.h"

AssimpConverter::AssimpConverter()
{
	aiLogStream stream;
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);
	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);
}


AssimpConverter::~AssimpConverter()
{
}

AssimpConverter* AssimpConverter::Instance()
{
	static AssimpConverter ac;
	return &ac;
}

Node* AssimpConverter::LoadModelFile( const std::string &filePath )
{
	std::string::size_type pos = filePath.find_last_of("/\\");
	if (pos != std::string::npos){
		_currentFolder = filePath.substr(0, pos + 1);
		//_fileName = _filePath.substr(pos + 1);
	}

	_scene = aiImportFile(filePath.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (!_scene) return nullptr;

	Node *root = new Node;
	convertaiNodeToNode(_scene->mRootNode, root);
	return root;
}

void AssimpConverter::convertaiNodeToNode( aiNode *ainode, Node *node )
{
	node->setMatrix(glm::mat4(ainode->mTransformation.a1, ainode->mTransformation.a2, ainode->mTransformation.a3, ainode->mTransformation.a3
		                    , ainode->mTransformation.b1, ainode->mTransformation.b2, ainode->mTransformation.b3, ainode->mTransformation.b4
		                    , ainode->mTransformation.c1, ainode->mTransformation.c2, ainode->mTransformation.c3, ainode->mTransformation.c4
		                    , ainode->mTransformation.d1, ainode->mTransformation.d2, ainode->mTransformation.d3, ainode->mTransformation.d4));
	for (unsigned int i = 0; i < ainode->mNumChildren; ++i){
		auto child = new Node;
		convertaiNodeToNode(ainode->mChildren[i], child);
		node->addChild(child);
	}

	for (unsigned int i = 0; i < ainode->mNumMeshes; ++i){
		auto geo = new Geometry;
		convertaiMeshToGeometry(_scene->mMeshes[ainode->mMeshes[i]], geo);
		node->attachRenderableObject(geo);
	}
}

void AssimpConverter::convertaiMeshToGeometry( aiMesh *aimesh, Geometry *geometry )
{
	if (aimesh->HasPositions()){
		Vec3Array vertexArray;
		for (unsigned int i = 0; i < aimesh->mNumVertices; ++i){

			vertexArray.push_back(glm::vec3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z));
		}
		geometry->setVertexArray(vertexArray);
	}

	if (aimesh->HasNormals()){
		Vec3Array normalArray;
		for (unsigned int i = 0; i < aimesh->mNumVertices; ++i){

			normalArray.push_back(glm::vec3(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z));
		}
		geometry->setNormalArray(normalArray);
	}

	if (aimesh->HasTangentsAndBitangents()){
		Vec3Array tangentArray;
		for (unsigned int i = 0; i < aimesh->mNumVertices; ++i){

			tangentArray.push_back(glm::vec3(aimesh->mTangents[i].x, aimesh->mTangents[i].y, aimesh->mTangents[i].z));
		}
		geometry->setTangentArray(tangentArray);

		Vec3Array bitangentArray;
		for (unsigned int i = 0; i < aimesh->mNumVertices; ++i){

			bitangentArray.push_back(glm::vec3(aimesh->mBitangents[i].x, aimesh->mBitangents[i].y, aimesh->mBitangents[i].z));
		}
		geometry->setBitangentArray(bitangentArray);
	}

	if (aimesh->HasVertexColors(0)){
		Vec4Array colorArray;
		for (unsigned int i = 0; i < aimesh->mNumVertices; ++i){
			colorArray.push_back(glm::vec4(aimesh->mColors[0][i].r, aimesh->mColors[0][i].g, aimesh->mColors[0][i].b, aimesh->mColors[0][i].a));
		}
		geometry->setColorArray(colorArray);
	}

	if (aimesh->HasFaces()){
		UIntArray pointIndexArray;
		UIntArray lineIndexArray;
		UIntArray triIndexArray;
		for (unsigned int i = 0; i < aimesh->mNumFaces; ++i){
			const aiFace &face = aimesh->mFaces[i];
			switch (face.mNumIndices)
			{
			case 1:
				{
					pointIndexArray.push_back(face.mIndices[0]);
				}
				break;

			case 2:
				{
					lineIndexArray.push_back(face.mIndices[0]);
					lineIndexArray.push_back(face.mIndices[1]);
				}
				break;

			case 3:
				{
					triIndexArray.push_back(face.mIndices[0]);
					triIndexArray.push_back(face.mIndices[1]);
					triIndexArray.push_back(face.mIndices[2]);
				}
				break;

			default:
				break;
			}
		}

		Geometry::IndicesArray indicesArray;
		if (!pointIndexArray.empty())
			indicesArray.push_back(std::make_pair(Geometry::GT_POINTS, pointIndexArray));
		if (!lineIndexArray.empty())
			indicesArray.push_back(std::make_pair(Geometry::GT_LINES, lineIndexArray));
		if (!triIndexArray.empty())
			indicesArray.push_back(std::make_pair(Geometry::GT_TRIANGLES, triIndexArray));

		geometry->setIndicesArray(indicesArray);
	}

	if (0 <= aimesh->mMaterialIndex){
		aiMaterial *aimat = _scene->mMaterials[aimesh->mMaterialIndex];
		Material *material = new Material;

		aiColor4D col;
		aiGetMaterialColor(aimat, AI_MATKEY_COLOR_DIFFUSE, &col);
		material->setDiffuseColor(glm::vec4(col.r, col.g, col.b, col.a));
		aiGetMaterialColor(aimat, AI_MATKEY_COLOR_SPECULAR, &col);
		material->setSpecularColor(glm::vec4(col.r, col.g, col.b, col.a));
		aiGetMaterialColor(aimat, AI_MATKEY_COLOR_AMBIENT, &col);
		material->setAmbientColor(glm::vec4(col.r, col.g, col.b, col.a));
		float shininess = 0.0f;
		unsigned int umax = 1;
		aiGetMaterialFloatArray(aimat, AI_MATKEY_SHININESS, &shininess, &umax);

		GLuint texID;
		aiString texPath;
		aimat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		if (texPath.length > 0){
			TextureManager::Instance()->LoadTexture(texID, (_currentFolder + std::string(texPath.C_Str())).c_str());
			material->setDiffuseTexture(texID);
		}

		aimat->GetTexture(aiTextureType_SPECULAR, 0, &texPath);
		if (texPath.length > 0){
			TextureManager::Instance()->LoadTexture(texID, (_currentFolder + std::string(texPath.C_Str())).c_str());
			material->setSpecularTexture(texID);
		}

		aimat->GetTexture(aiTextureType_AMBIENT, 0, &texPath);
		if (texPath.length > 0){
			TextureManager::Instance()->LoadTexture(texID, (_currentFolder + std::string(texPath.C_Str())).c_str());
			material->setAmbientTexture(texID);
		}
	}
}
