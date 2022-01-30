#version 330 core

uniform vec4 base_color;

in vec4 gPos;
in vec3 bc_dist;
out vec4 output_color;

void main()
{	
	const float threshold = 0.15f;
	float nearD = min(min(bc_dist.x,bc_dist.y),bc_dist.z);
	float alpha = (nearD > threshold) ? 0.0f : (threshold-nearD)/threshold;
	//float nearD = bc_dist.y;
	//float edgeIntensity = exp2(-1.0*nearD*nearD);
	//float dist = distance(gPos,gl_FragCoord.xyz);
	output_color=vec4(base_color.x,base_color.y,base_color.z,alpha);
}
