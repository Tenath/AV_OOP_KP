#version 330 core

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

#define MAX_POINT_LIGHTS 20
uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform int PointLightCount;

uniform Material material;
uniform vec3 camera_pos;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

out vec4 OutputColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse /*+ specular*/);
}

void main()
{
	vec3 result = vec3(0.0f,0.0f,0.0f);
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camera_pos - FragPos);

    // Только точечное освещение
    for(int i = 0; i < PointLightCount; i++)
	{
        result += CalcPointLight(PointLights[i], norm, FragPos, viewDir);    
	}
	
	//vec3 result = vec3(0.5f,0.5f,0.5f);
    OutputColor = vec4(result, 1.0f);
}