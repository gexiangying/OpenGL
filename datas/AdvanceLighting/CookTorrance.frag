#version 400
layout(location=0) out vec4 FragColor;
uniform vec4 LightColor;
uniform vec4 DiffColor;
uniform vec4 SpecColor;
uniform float Refractiable;
uniform float Dm;
uniform float Dc;
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
	
	float G = min(1.0, 2.0 * (NdotH * NdotV) / HdotV);
	G = min(G, 2.0 * (NdotH * NdotL) / HdotV);
	
//	float g = sqrt(Refractiable * Refractiable + HdotV * HdotV - 1);
//	float gaddc = g + HdotV;
//	float gsubc = g - HdotV;
//	float a = (HdotV * gaddc - 1) * (HdotV * gaddc - 1);
//	float b = (HdotV * gaddc + 1) * (HdotV * gaddc + 1);
//	float F = (gsubc * gsubc) / (2 * gaddc * gaddc) * ( 1 + a / b );

	float F = Refractiable + (1.0 - Refractiable) * pow(1.0 - NdotL, 5.0);
	
//	float ePow = -pow( tan(acos(NdotH)) / Dm, 2 );
//	float D = exp(ePow) / ( (Dm * Dm) * pow(NdotH, 4) );
	
	float D = Dc * exp( -pow(acos(NdotH) / Dm, 2) );
	
	vec4 color = DiffColor * max(0.0, NdotL) * LightColor * (F * D * G) / (NdotL * NdotV);
	FragColor = vec4(color); 
}