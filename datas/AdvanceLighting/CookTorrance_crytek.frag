#version 430
layout(location=0) out vec4 FragColor;
uniform vec4 LightColor;
uniform vec4 DiffColor;
uniform vec4 SpecColor;
uniform float Refractiable;
uniform float smoothness;
const float PI = 3.1415926;
in vec3 eNormal;
in vec4 eVertex;
in vec4 eLightPos;

void main()
{
	vec3 n = normalize(eNormal);
	vec3 ldir = normalize(eLightPos.xyz - eVertex.xyz);
	vec3 vdir = normalize(-eVertex.xyz);
	vec3 H = normalize(ldir + vdir);
	float NdotL = dot(n, ldir);
	float NdotV = dot(n, vdir);
	float HdotV = dot(H, vdir);
	float NdotH = dot(n, H);
	float alpha = pow(1.0 - smoothness * 0.7, 6.0);
	float alpha2 = alpha * alpha;

	float k = (0.8 + 0.5 * alpha) * (0.8 + 0.5 * alpha) / 2.0;
	float Gl = NdotL / (NdotL * (1.0 - k) + k);
	float Gv = NdotV / (NdotV * (1.0 - k) + k);
	float G = Gl * Gv;

	float F = Refractiable + (1.0 - Refractiable) * pow(1.0 - HdotV, 5.0);
	
	float Db = PI * pow(NdotH * NdotH * (alpha2 - 1) + 1, 2.0);
	float D = alpha2 / Db; 
	
	vec4 color = DiffColor * max(0.0, NdotL) * LightColor * (F * D * G) / (4.0f * NdotL * NdotV);
	//vec4 color = LightColor * (F * D * G) / (4.0f * NdotL * NdotV);
	FragColor = vec4(color); 
}