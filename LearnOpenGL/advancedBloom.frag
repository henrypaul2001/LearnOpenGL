#version 330 core

out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform sampler2D dirtMask;

uniform float bloomStrength;
uniform float dirtMaskStrength;

vec3 MixBloom() {
	vec3 hdr = texture(screenTexture, TexCoords).rgb;
	vec3 bloom = texture(bloomTexture, TexCoords).rgb;
	vec3 dirt = texture(dirtMask, TexCoords).rgb * dirtMaskStrength;
	return mix(hdr, bloom + (bloom * dirt), vec3(bloomStrength));
}

void main() {
	FragColour = vec4(MixBloom(), 1.0);
}