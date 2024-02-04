#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
uniform vec2 translate;
uniform vec2 scale;

void main()
{
	gl_Position = vec4((position * scale) + translate, 0.0f, 1.0f);
	TexCoords = texCoords;
}