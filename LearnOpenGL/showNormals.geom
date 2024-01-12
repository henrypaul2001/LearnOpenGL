#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
	mat4 Projection;
} gs_in[];

const float magnitude = 0.1;

void GenerateLine(int index) {
	gl_Position = gs_in[index].Projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gs_in[index].Projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 1.0) * magnitude);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0); // first vertex normal
	GenerateLine(1); // second vertex normal
	GenerateLine(2); // third vertex normal
}