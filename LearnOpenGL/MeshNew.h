#pragma once
#include <vector>
#include "Mesh.h"

#define MAX_BONE_INFLUENCE 8

struct VertexNew {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	//std::vector<BoneNew> Bones;
	// bone indexes which will influence this vertex
	//int m_BoneIDs[MAX_BONE_INFLUENCE];
	// weights from each bone
	//float m_Weights[MAX_BONE_INFLUENCE];
};

class MeshNew
{
public:
	std::vector<VertexNew> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

	MeshNew(std::vector<VertexNew> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
private:
	unsigned int VBO, EBO;
	void setupMesh();
};
};