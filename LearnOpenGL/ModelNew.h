#pragma once
#include "MeshNew.h"
#include "Model.h"
class ModelNew
{
public:
	ModelNew(const char* filepath) {
		loadModel(filepath);
	}
	void Draw(Shader& shader);

	std::vector<MeshNew> meshes;
	std::vector<Texture> textures_loaded;

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

	//auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	//int& GetBoneCount() { return m_BoneCounter; }
private:
	//const int MAX_BONE_WEIGHTS = 8;

	std::string directory;

	//std::map<std::string, BoneInfo> m_BoneInfoMap;
	//int m_BoneCounter = 0;

	//void SetVertexBoneDataToDefault(Vertex& vertex) {
	//	for (int i = 0; i < MAX_BONE_WEIGHTS; i++) {
	//		vertex.m_BoneIDs[i] = -1;
	//		vertex.m_Weights[i] = 0.0f;
	//	}
	//}

	//void SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	//{
	//	for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
	//	{
	//		if (vertex.m_BoneIDs[i] < 0)
	//		{
	//			vertex.m_Weights[i] = weight;
	//			vertex.m_BoneIDs[i] = boneID;
	//			break;
	//		}
	//	}
	//}

	//void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void loadModel(std::string filepath);
	void processNode(aiNode* node, const aiScene* scene);
	MeshNew processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};