#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
	// store fragment position vector in the first gbuffer texture_diffuse1
	gPosition = FragPos;

	// store per-fragment normals in gbuffer
	gNormal = normalize(Normal);

	// store diffuse per fragment colour in gbuffer
	gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;

	// store specular intensity in gAlbedoSpec's alpha component
	gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}