#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

vec4 NoEffect() {
	return texture(screenTexture, TexCoords);
}

vec4 Inversion() {
	return vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}

vec4 Grayscale() {
	float redWeight = 0.2126;
	float greenWeight = 0.7152;
	float blueWeight = 0.0722;

	vec4 result = texture(screenTexture, TexCoords);
	float average = (redWeight * result.r) + (greenWeight * result.g) + (blueWeight * result.b);

	return vec4(average, average, average, 1.0);
}

void main() {
	FragColor = Grayscale();
}