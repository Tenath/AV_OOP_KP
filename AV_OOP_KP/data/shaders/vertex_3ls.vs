#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 pos = vec4(position.xyz, 1.0f);
	mat4 mvp_matrix = projection_matrix*view_matrix*model_matrix;
	gl_Position = mvp_matrix * pos;
    
    FragPos = vec3(model_matrix * pos);
    Normal = mat3(transpose(inverse(model_matrix))) * normal;
    //LightPos = vec3(view_matrix * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
}
