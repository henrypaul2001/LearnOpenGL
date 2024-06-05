#version 330 core
layout (location = 0) in vec4 vertex; // vec2 position, vec2 texcoords

out vec2 TexCoords;
out vec4 ParticleColour;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 colour;

void main() {
	float scale = 10.0f;
	TexCoords = vertex.zw;
	ParticleColour = colour;
	gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}