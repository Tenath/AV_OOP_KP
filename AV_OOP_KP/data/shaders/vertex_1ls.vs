#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
//layout(location = 2) in vec4 color;

uniform mat3 rm;
uniform float aspect_ratio;
uniform float scale;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_pos;

uniform vec4 base_color;

out vec3 v_normal;
out vec4 pos;
out vec4 interp_color;
out vec3 world_pos;
out vec3 LightPos;

void main()
{
	pos = vec4(position.xyz, 1.0f);
	mat4 mvp_matrix = projection_matrix*view_matrix*model_matrix;
	gl_Position = mvp_matrix * pos;

	v_normal = normal;
	world_pos = (model_matrix*position).xyz;
	LightPos = vec3(view_matrix * vec4(light_pos, 1.0f));
}
