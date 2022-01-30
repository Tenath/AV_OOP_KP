#version 330 core

uniform vec4 base_color;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 ambient;

in vec4 gPos;
in vec3 LightPos;
out vec4 output_color;

//in vec4 interp_color;
in vec3 v_normal;
in vec3 world_pos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 camera_pos;

void main()
{		
	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(LightPos-world_pos);
	
	// diffuse
	float diff = max(dot(normal,light_dir),0.0f);
	vec3 diffuse = diff*light_color.xyz;
	
	//specular
	const float specular_str = 0.5f;
	vec3 view_dir = normalize(camera_pos-world_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f),8);
	vec3 specular = specular_str * spec * light_color;
	
	vec3 result = (ambient+diffuse+specular)*material.diffuse;
	output_color = vec4(result.xyz,1.0f);
	//output_color=vec4(interp_color.x,interp_color.y,interp_color.z,1.0f);
}
