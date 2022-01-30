#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;

uniform float aspect_ratio;
uniform float scale;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec4 pos;
out vec4 interp_color;

void main()
{
	pos = vec4(position.x, position.y, position.z, 1.0f);
	mat4 mvp_matrix = projection_matrix*view_matrix*model_matrix;
	gl_Position = mvp_matrix * pos;
	
	//gl_Position.x = gl_Position.x/aspect_ratio;
	/*vec3 rotated = rm * position;

    gl_Position.x = scale*rotated.x/aspect_ratio;
	gl_Position.y = scale*rotated.y;
	gl_Position.z = scale*rotated.z;
	gl_Position.w = 1.0f;*/
	
	//pos = position;
}
