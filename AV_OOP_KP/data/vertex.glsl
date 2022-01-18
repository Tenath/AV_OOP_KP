#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;

uniform mat4 rm;
uniform float aspect_ratio;

out vec3 pos;
out vec4 col;

void main()
{
	vec4 rotated = rm * vec4(position.x,position.y,position.z,1.0f);
	
	gl_Position.x = rotated.x / aspect_ratio;
	gl_Position.y = rotated.y;
	gl_Position.z = rotated.z;
	gl_Position.w = 1.0f;
	
	pos = position;
	col = color;
}