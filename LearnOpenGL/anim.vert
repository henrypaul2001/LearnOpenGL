#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 boneTransforms[MAX_BONES];

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main() {
	vec4 totalPosition = vec4(0.0);
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
		if (boneIds[i] == -1) {
			continue;
		}
		if (boneIds[i] >= MAX_BONES) {
			totalPosition = vec4(pos, 1.0);
			break;
		}
		vec4 localPosition = boneTransforms[boneIds[i]] * vec4(pos, 1.0);
		totalPosition += localPosition * weights[i];
		vec3 localNormal = mat3(boneTransforms[boneIds[i]]) * normal;
	}

	mat4 viewModel = view * model;
	TexCoords = texCoords;
	FragPos = vec3(model * totalPosition);
	Normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = projection * viewModel * totalPosition;
}