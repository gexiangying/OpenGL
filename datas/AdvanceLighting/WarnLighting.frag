#version 430
layout(location=0) out vec4 FragColor;
uniform vec4 LightColor;
uniform float DiffDensity;
uniform float WarnExponent;
in vec3 eNormal;
in vec4 eVertex;
in vec4 eLightPos;
in vec4 eOrigin;

vec3 GetLightDirection(in vec4 ePosition)
{
	return normalize(eLightPos.xyz - ePosition.xyz);
}

vec4 GetLightIntensity(in vec4 ePosition)
{
	vec3 sq = (eLightPos.xyz - ePosition.xyz) * (eLightPos.xyz - ePosition.xyz);
	float dis2 = sq.x + sq.y + sq.z;
	return LightColor / dis2;
}

void main()
{
	vec3 ldir = GetLightDirection(eVertex);
	vec4 lint = GetLightIntensity(eVertex);
	float intensity = pow(dot(normalize(eLightPos.xyz - eOrigin.xyz), ldir), WarnExponent);
	vec4 color = lint * DiffDensity * intensity * max(0.0, dot(ldir, eNormal));
	FragColor = vec4(color);
}