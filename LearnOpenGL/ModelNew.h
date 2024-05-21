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

#define MAX_BONE_INFLUENCE 8;

struct BoneNew {
	int BoneID = -1;
	glm::mat4 offsetMatrix = glm::mat4(1.0f);

	std::vector<std::string> childBoneNames;
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

	const std::unordered_map<std::string, BoneNew>& GetBones() const { return bones; }
	const int GetNumBones() const { return bones.size(); }
private:
	std::string directory;

	std::unordered_map<std::string, BoneNew> bones;

	void processBones(std::vector<VertexNew>& vertices, aiMesh* mesh, const aiScene* scene);
	//void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void loadModel(std::string filepath);
	void processNode(aiNode* node, const aiScene* scene);
	MeshNew processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureNew> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};