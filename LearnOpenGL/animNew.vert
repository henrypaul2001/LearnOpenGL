#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 8;
uniform mat4 boneTransforms[MAX_BONES];

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main() {
	vec4 transformedLocalPos = vec4(0.0);
	vec3 transformedNormal = vec3(0.0);

	mat4 boneTransform = mat4(0.0);
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
		if (boneIDs[i] != -1 && boneIDs[i] < MAX_BONES) {
			boneTransform += boneTransforms[boneIDs[i]] * weights[i];
		}
	}

	transformedLocalPos = boneTransform * vec4(pos, 1.0);
	transformedNormal = vec3(boneTransform * vec4(normal, 1.0));

	mat4 viewModel = view * model;
	TexCoords = texCoords;
	FragPos = vec3(model * transformedLocalPos);
	Normal = mat3(transpose(inverse(model))) * transformedNormal;
	gl_Position = projection * viewModel * transformedLocalPos;
}