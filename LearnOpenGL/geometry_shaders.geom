#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

void BuildHouse(vec4 position)
{
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // bottom left
	EmitVertex();

	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // bottom right
	EmitVertex();

	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); // top left
	EmitVertex();

	gl_Position = position + vec4( 0.2, 0.2, 0.0, 0.0); // top right
	EmitVertex();

	gl_Position = position + vec4( 0.0, 0.4, 0.0, 0.0); // top
	EmitVertex();

	EndPrimitive();
}

void main() 
{
	BuildHouse(gl_in[0].gl_Position);
}