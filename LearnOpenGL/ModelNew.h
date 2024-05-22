#pragma once
#include "MeshNew.h"
//#include "Model.h"
#include <unordered_map>

#include "Shader.h"
#include "stb_image.h"

#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>

struct BoneNew {
	int BoneID = -1;
	glm::mat4 offsetMatrix = glm::mat4(1.0f);
	glm::mat4 finalTransform = glm::mat4(1.0f);
	glm::mat4 nodeTransform = glm::mat4(1.0f);

	std::vector<std::string> childNodeNames;
};

class ModelNew
{
public:
	ModelNew(const char* filepath) {
		loadModel(filepath);
	}
	void Draw(Shader& shader);

	std::vector<MeshNew> meshes;
	std::vector<TextureNew> textures_loaded;

	static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	const std::map<std::string, BoneNew>& GetBones() const { return bones; }
	const BoneNew* GetRootBone() const { return rootBone; }
	const int GetNumBones() const { return bones.size(); }
	void GetBoneTransforms(std::vector<glm::mat4>& out_transforms);
private:
	std::string directory;

	std::map<std::string, BoneNew> bones;
	BoneNew* rootBone;

	void ProcessChildBonesFinalTransformRecursive(BoneNew& bone, const glm::mat4& parentTransform);

	void processBones(std::vector<VertexNew>& vertices, aiMesh* mesh, const aiScene* scene);
	//void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void loadModel(std::string filepath);
	void processNode(aiNode* node, const aiScene* scene);
	MeshNew processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureNew> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};