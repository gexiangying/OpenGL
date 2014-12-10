#version 400
layout(location=0) out vec4 FragColor;
uniform vec4 LightColor;
uniform vec4 DiffColor;
uniform vec4 SpecColor;
uniform float Shiness;
in vec3 eNormal;
in vec4 eVertex;
in vec4 eLightPos;

void main()
{
	vec3 n = normalize(eNormal);
	vec3 ldir = normalize(eLightPos.xyz - eVertex.xyz);
	vec3 vdir = normalize(-eVertex.xyz);
	float NdotL = dot(n, ldir);
	float NdotV = dot(n, vdir);
	vec3 R = normalize(2 * n * NdotL - ldir);
	float RdotV = dot(R, vdir);
	
	vec4 color = DiffColor * LightColor * max(0.0, NdotL) + SpecColor * LightColor * (RdotV / (Shiness - ( (Shiness - 1) * RdotV )));
	
	FragColor = vec4(color); 
}