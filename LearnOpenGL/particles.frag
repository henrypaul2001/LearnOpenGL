#version 330 core
in vec2 TexCoords;
in vec4 ParticleColour;

out vec4 Colour;

uniform sampler2D sprite;

void main() {
	Colour = (texture(sprite, TexCoords) * ParticleColour);
}