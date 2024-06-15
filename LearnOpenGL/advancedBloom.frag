#version 330 core

out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

uniform float bloomStrength;

vec3 MixBloom() {
	vec3 hdr = texture(screenTexture, TexCoords).rgb;
	vec3 bloom = texture(bloomTexture, TexCoords).rgb;
	return mix(hdr, bloom, vec3(bloomStrength));
}

void main() {
	FragColour = vec4(MixBloom(), 1.0);
}