#version 330 core

// This shader upsamples a texture, taken from the Call of Duty: Advanced Warfare method presented at ACM Siggraph 2014

uniform sampler2D srcTexture;
uniform float filterRadius;

in vec2 texCoord;

layout (location = 0) out vec3 upsample;

void main() {
	// Filter kernel is applied with a radius, specified in texture coords, so that radius will vary across mip resolutions
	float x = filterRadius;
	float y = filterRadius;

	// Take 9 samples around current texel
	// a - b - c
    // d - e - f
    // g - h - i
	// (e is current texel)
	vec3 a = texture(srcTexture, vec2(texCoord.x - x, texCoord.y + y)).rgb;
    vec3 b = texture(srcTexture, vec2(texCoord.x,     texCoord.y + y)).rgb;
    vec3 c = texture(srcTexture, vec2(texCoord.x + x, texCoord.y + y)).rgb;

    vec3 d = texture(srcTexture, vec2(texCoord.x - x, texCoord.y)).rgb;
    vec3 e = texture(srcTexture, vec2(texCoord.x,     texCoord.y)).rgb;
    vec3 f = texture(srcTexture, vec2(texCoord.x + x, texCoord.y)).rgb;

    vec3 g = texture(srcTexture, vec2(texCoord.x - x, texCoord.y - y)).rgb;
    vec3 h = texture(srcTexture, vec2(texCoord.x,     texCoord.y - y)).rgb;
    vec3 i = texture(srcTexture, vec2(texCoord.x + x, texCoord.y - y)).rgb;

    // Weighted distribution using 3x3 tent filter
    // 1    | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    upsample = e * 4.0;
    upsample += (b + d + f + h) * 2.0;
    upsample += (a + c + g + i);
    upsample += 1.0 / 16.0;
}