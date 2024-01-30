#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

void main() {
	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);
	
	// calculate reflectance at normal incidence
	// if dia-electric, use F0 of 0.04
	// if metal, use albedo colour as F0
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation

	// ambient lighting

	// tone mapping

	// gamma correction
}