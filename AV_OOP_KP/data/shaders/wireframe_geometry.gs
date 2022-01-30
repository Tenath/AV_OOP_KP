#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

//in vec3 pos[3];
out vec4 gPos;
out vec3 bc_dist;

vec3 GetBarycentricDist(int index, float area, vec2 v)
{
	vec3 result=vec3(0.0f,0.0f,0.0f);
	result;
	if(length(v)==0.0f) return result;
	float value = area/length(v);

	switch(index)
	{
		case 0: result.x=value; break;
		case 1: result.y=value; break;
		case 2: result.z=value; break;
	}
	return result;
}

void main()
{
	vec2 p[3];
	for(int i=0; i<3; i++)
	{
		p[i] = gl_in[i].gl_Position.xy;
	}
	
	vec2 v[3];
	v[0]=p[2]-p[1];
	v[1]=p[2]-p[0];
	v[2]=p[1]-p[0];
	
	float area = abs(v[1].x*v[2].y - v[1].y*v[2].x);
	
	for(int i=0; i<3;i++)
	{
		gl_Position = gl_in[i].gl_Position;
		gPos=gl_Position;
		bc_dist = GetBarycentricDist(i,area,v[i]);
		//bc_dist=vec3(1.0f,1.0f,1.0f);
		EmitVertex();
	}
	
	EndPrimitive();
}