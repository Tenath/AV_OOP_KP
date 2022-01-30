#version 330 core

uniform vec4 base_color;

in vec4 gPos;
out vec4 output_color;

in vec4 interp_color;

void main()
{	
	output_color=vec4(base_color.x,base_color.y,base_color.z,1.0f);
}
