#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 texCoords;
    out mat4 Projection;
} vs_out;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vs_out.texCoords = aTexCoords;
    vs_out.Projection = projection;
    gl_Position = view * model * vec4(aPos, 1.0); 
}