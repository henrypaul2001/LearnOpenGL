#include "ModelNew.h"

void ModelNew::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void ModelNew::loadModel(std::string filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = filepath.substr(0, filepath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void ModelNew::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

MeshNew ModelNew::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexNew> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureNew> textures;

	// retrieve vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		VertexNew vertex;

		//SetVertexBoneDataToDefault(vertex);

		// retrieve positions
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// retrieve normals
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// retrieve texcoords
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// tangent
			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;
			// bitangent
			vertex.Bitangent.x = mesh->mBitangents[i].x;
			vertex.Bitangent.y = mesh->mBitangents[i].y;
			vertex.Bitangent.z = mesh->mBitangents[i].z;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// retrieve indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	bool PBR = false;
	// retrieve materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (!PBR) {
			std::vector<TextureNew> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<TextureNew> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"); // roughness map
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<TextureNew> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<TextureNew> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height"); // AO
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		else {
			// PBR
			std::vector<TextureNew> albedoMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "albedoMap");
			textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());

			std::vector<TextureNew> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normalMap");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<TextureNew> metallicMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "metallicMap");
			textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

			std::vector<TextureNew> roughnessMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "roughnessMap");
			textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

			std::vector<TextureNew> aoMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "aoMap");
			textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
		}
	}

	// retrieve bones
	if (mesh->HasBones()) {
		processBones(vertices, mesh, scene);
	}

	return MeshNew(vertices, indices, textures);
}

void ModelNew::GetBoneTransforms(std::vector<glm::mat4>& out_transforms)
{
	out_transforms.resize(bones.size());

	ProcessChildBonesFinalTransformRecursive(*rootBone, skeletonOriginTransform);

	auto it = bones.begin();
	for (unsigned int i = 0; i < bones.size(); i++) {
		out_transforms[i] = it->second.finalTransform;
		it++;
	}
}

void ModelNew::ProcessChildBonesFinalTransformRecursive(BoneNew& bone, const glm::mat4& parentTransform)
{
	glm::mat4 nodeTransform = bone.nodeTransform;

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	bone.finalTransform = globalTransform * bone.offsetMatrix;

	for (std::string& boneName : bone.childNodeNames) {
		if (bones.find(boneName) != bones.end()) {
			BoneNew& child = bones[boneName];
			ProcessChildBonesFinalTransformRecursive(child, globalTransform);
		}
	}
}

void ModelNew::processBones(std::vector<VertexNew>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		// Get boneID
		// ----------
		int boneID = -1;
		std::string boneName = mesh->mBones[i]->mName.C_Str();
		
		if (bones.find(boneName) == bones.end()) {
			// Create new bone
			BoneNew newBone;
			boneID = bones.size();
			newBone.BoneID = boneID;
			newBone.offsetMatrix = ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);
			newBone.nodeTransform = ConvertMatrixToGLMFormat(mesh->mBones[i]->mNode->mTransformation);
			newBone.Name = boneName;

			// Get child bone names
			for (unsigned int j = 0; j < mesh->mBones[i]->mNode->mNumChildren; j++) {
				std::string childName = mesh->mBones[i]->mNode->mChildren[j]->mName.C_Str();
				newBone.childNodeNames.push_back(childName);
			}

			bones[boneName] = newBone;

			if (bones.find(mesh->mBones[i]->mNode->mParent->mName.C_Str()) == bones.end()) {
				// Parent node name is not in bones list, this bone must be the root bone
				rootBone = &bones[boneName];
				skeletonOriginTransform = ConvertMatrixToGLMFormat(mesh->mBones[i]->mNode->mParent->mTransformation);
			}
		}
		else {
			// Bone already exists
			boneID = bones[boneName].BoneID;
		}

		// Update vertices with IDs and weights
		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			const aiVertexWeight& weight = mesh->mBones[i]->mWeights[j];
			int vertexID = weight.mVertexId;
			assert(vertexID < vertices.size());
			vertices[vertexID].AddBoneData(boneID, weight.mWeight);
		}
	}
}

std::vector<TextureNew> ModelNew::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureNew> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].filepath.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			TextureNew texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.filepath = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int ModelNew::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
