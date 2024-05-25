#pragma once
#include <vector>
//#include "Mesh.h"
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

#define MAX_BONE_INFLUENCE 8

struct VertexNew {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	int BoneIDs[MAX_BONE_INFLUENCE] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	float BoneWeights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	void AddBoneData(int boneID, float boneWeight) {

		if (boneWeight == 0.0f) {
			// skip bone
			return;
		}

		for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
			if (BoneIDs[i] == -1 && BoneIDs[i] != boneID) {
				// empty bone slot found
				BoneIDs[i] = boneID;
				BoneWeights[i] = boneWeight;
				return;
			}
		}

		// if this line is reached, then there are too many bones
		assert(0);
	};
};

struct TextureNew {
	unsigned int id;
	std::string type;
	std::string filepath;
};

class MeshNew
{
public:
	std::vector<VertexNew> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureNew> textures;
	unsigned int VAO;

	MeshNew(std::vector<VertexNew> vertices, std::vector<unsigned int> indices, std::vector<TextureNew> textures);
	void Draw(Shader& shader);
private:
	unsigned int VBO, EBO;
	void setupMesh();
};