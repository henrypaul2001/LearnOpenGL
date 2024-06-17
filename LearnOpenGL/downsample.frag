#version 330 core

// This shader downsamples a texture, taken from the Call of Duty: Advanced Warfare method presented at ACM Siggraph 2014
// Was designed to eliminate "pulsating artifacts and temporal stability issues"

uniform sampler2D srcTexture;
uniform vec2 srcResolution;

uniform float threshold;
uniform float softThreshold;

in vec2 texCoord;
layout (location = 0) out vec3 downsample;

vec3 PowVec3(vec3 v, float p)
{
	return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float invGamma = 1.0 / 2.2;
vec3 ToSRGB(vec3 v) { return PowVec3(v, invGamma); }

float RGBToLuminance(vec3 colour)
{
	return dot(colour, vec3(0.2126, 0.71252, 0.0722));
}

float KarisAverage(vec3 colour)
{
	float luma = RGBToLuminance(ToSRGB(colour)) * 0.25;
	return 1.0 / (1.0 + luma);
}

uniform bool firstIteration;

void main() {
	vec2 srcTexelSize = 1.0 / srcResolution;
	float x = srcTexelSize.x;
	float y = srcTexelSize.y;

	// Take 13 samples around current texel
	// a - b - c
	// - j - k -
	// d - e - f
	// - l - m -
	// g - h - i
	// (e is current texel level)

	vec3 a = texture(srcTexture, vec2(texCoord.x - 2 * x, texCoord.y + 2 * y)).rgb;
	vec3 b = texture(srcTexture, vec2(texCoord.x, texCoord.y + 2 * y)).rgb;
	vec3 c = texture(srcTexture, vec2(texCoord.x + 2 * x, texCoord.y + 2 * y)).rgb;

	vec3 d = texture(srcTexture, vec2(texCoord.x - 2 * x, texCoord.y)).rgb;
	vec3 e = texture(srcTexture, vec2(texCoord.x, texCoord.y)).rgb;
	vec3 f = texture(srcTexture, vec2(texCoord.x + 2 * x, texCoord.y)).rgb;

	vec3 g = texture(srcTexture, vec2(texCoord.x - 2 * x, texCoord.y - 2 * y)).rgb;
	vec3 h = texture(srcTexture, vec2(texCoord.x, texCoord.y - 2 * y)).rgb;
	vec3 i = texture(srcTexture, vec2(texCoord.x + 2 * x, texCoord.y - 2 * y)).rgb;

	vec3 j = texture(srcTexture, vec2(texCoord.x - x, texCoord.y + y)).rgb;
	vec3 k = texture(srcTexture, vec2(texCoord.x + x, texCoord.y + y)).rgb;
	vec3 l = texture(srcTexture, vec2(texCoord.x - x, texCoord.y - y)).rgb;
	vec3 m = texture(srcTexture, vec2(texCoord.x + x, texCoord.y - y)).rgb;

	// Weighted distribution
	// 0.5 + 0.125 + 0.125 + 0.125 = 1
	// a,b,d,e * 0.125
	// b,c,e,f * 0.125
	// d,e,g,h * 0.125
	// e,f,h,i * 0.125
	// j,k,l,m * 0.5


	// Helps alleviate firefly artifacts using karis average
	if (firstIteration) {
		// Writing to mip level 0, apply karis average to each block of 4 samples to prevent fireflies
		vec3 groups[5];
		groups[0] = (a + b + d + e) * (0.125f / 4.0f);
		groups[1] = (b + c + e + f) * (0.125f / 4.0f);
		groups[2] = (d + e + g + h) * (0.125f / 4.0f);
		groups[3] = (e + f + h + i) * (0.125f / 4.0f);
		groups[4] = (j + k + l + m) * (0.5f / 4.0f);
		
		groups[0] *= KarisAverage(groups[0]);
		groups[1] *= KarisAverage(groups[1]);
		groups[2] *= KarisAverage(groups[2]);
		groups[3] *= KarisAverage(groups[3]);
		groups[4] *= KarisAverage(groups[4]);
		downsample = groups[0] + groups[1] + groups[2] + groups[3] + groups[4];

		float brightness = max(downsample.r, max(downsample.g, downsample.b));
		float knee = threshold * softThreshold;
		float soft = brightness - threshold + knee;
		soft = clamp(soft, 0, 2.0 * knee);
		soft = soft * soft / (4.0 * knee + 0.00001);

		float contribution = max(soft, brightness - threshold);
		contribution /= max(brightness, 0.00001);
		downsample *= contribution;
	}
	else {
		// 0.125 * 5 + 0.03125 * 4 + 0.0625 * 4 = 1
		downsample = e * 0.125;
		downsample += (a + c + g + i) * 0.03125;
		downsample += (b + d + f + h) * 0.0625;
		downsample += (j + k + l + m) * 0.125;
	}
	downsample = max(downsample, 0.0001);
}