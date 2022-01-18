#version 330 core

in vec3 pos;
in vec4 col;

out vec4 output_color;

void main()
{
	output_color = vec4(col.x,col.y,col.z,1.0f);
	//output_color = vec4(1.0f,0.0f,0.0f,1.0f);
}