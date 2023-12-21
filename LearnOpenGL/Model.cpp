#include "Model.h"

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string filepath)
{
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	return Mesh();
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	return std::vector<Texture>();
}
