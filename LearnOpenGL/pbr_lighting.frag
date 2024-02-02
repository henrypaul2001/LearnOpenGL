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

// material textures
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// material flags
uniform bool useAlbedoMap;
uniform bool useNormalMap;
uniform bool useMetallicMap;
uniform bool useRoughnessMap;
uniform bool useAoMap;

// IBL
uniform samplerCube irradianceMap;
uniform bool useIrradianceMap;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main() {
	vec3 Albedo = useAlbedoMap ? pow(texture(albedoMap, TexCoords).rgb, vec3(2.2)) : albedo;
	vec3 normal = useNormalMap ? getNormalFromMap() : Normal;
	float Metallic = useMetallicMap ? texture(metallicMap, TexCoords).r : metallic;
	float Roughness = useRoughnessMap ? texture(roughnessMap, TexCoords).r : roughness;
	float AO = useAoMap ? texture(aoMap, TexCoords).r : ao;

	vec3 N = normalize(normal);
	vec3 V = normalize(camPos - WorldPos);

	// calculate reflectance at normal incidence
	// if dia-electric, use F0 of 0.04
	// if metal, use albedo colour as F0
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, Albedo, Metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < 4; ++i) {
		// per-light radiance
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, Roughness);
		float G = GeometrySmith(N, V, L, Roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
		vec3 specular = numerator / denominator;

		// kS is equal to fresnel
		vec3 kS = F;
		
		// for energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light)
		// to preserve this relationship the diffuse component (kD) should equal 1.0 - kS
		vec3 kD = vec3(1.0) - kS;

		// multiply kD by inverse metalness such that only non-metals have diffuse lighting , or linear blend if partly metal
		kD *= 1.0 - Metallic;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
	}

	// ambient lighting
	vec3 ambient = vec3(0.01) * Albedo * AO;
	if (useIrradianceMap) {
		vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - Metallic;
		vec3 irradiance = texture(irradianceMap, N).rgb;
		vec3 diffuse = irradiance * Albedo;
		ambient = (kD * diffuse) * AO;
	}

	vec3 color = ambient + Lo;

	// tone mapping
	color = color / (color + vec3(1.0));

	// gamma correction
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}