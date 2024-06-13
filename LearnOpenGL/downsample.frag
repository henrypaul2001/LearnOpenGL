#version 330 core

// This shader downsamples a texture, taken from the Call of Duty: Advanced Warfare method presented at ACM Siggraph 2014
// Was designed to eliminate "pulsating artifacts and temporal stability issues"

uniform sampler2D srcTexture;
uniform vec2 srcResolution;

in vec2 texCoord;
layout (location = 0) out vec3 downsample;

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

	// 0.125 * 5 + 0.03125 * 4 + 0.0625 * 4 = 1
	downsample = e * 0.125;
	downsample += (a + c + g + i) * 0.03125;
	downsample += (b + d + f + h) * 0.0625;
	downsample += (j + k + l + m) * 0.125;
}