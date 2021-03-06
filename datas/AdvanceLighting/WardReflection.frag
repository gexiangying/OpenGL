#version 430
layout(location=0) out vec4 FragColor;
uniform vec4 LightColor;
uniform vec4 DiffColor;
uniform vec4 SpecColor;
uniform float roughness;
uniform float roughnessx;
uniform float roughnessy;
uniform int isotropicOranisotropic;
const float PI = 3.1415926;
in vec3 eNormal;
in vec4 eVertex;
in vec4 eLightPos;

void main()
{
	vec3 n = normalize(eNormal);
	vec3 ldir = normalize(eLightPos.xyz - eVertex.xyz);
	vec3 vdir = normalize(-eVertex.xyz);
	vec3 halfdir = normalize(ldir + vdir);
	float NdotL = dot(n, ldir);
	float NdotV = dot(n, vdir);
	
	vec4 color;
	if (isotropicOranisotropic == 0)
	{
		float roughness2 = roughness * roughness;
		float gama = acos(dot(n, halfdir));
		float para1 = exp(-tan(gama) * tan(gama) / roughness2);
		float para2 = 2 * PI * roughness2 * sqrt(NdotL * NdotV);
		color = (DiffColor + SpecColor *  (para1 / para2)) * max(0.0, NdotL) * LightColor;
	}
	else
	{
		if (isotropicOranisotropic == 1)
		{
			vec3 dir = vec3(0.0, 0.0, 1.0);
			vec3 x = normalize(cross(n, dir));
			vec3 y = normalize(cross(n, x));
			float hdotx = dot(halfdir, x);
			float hdoty = dot(halfdir, y);
			float hdotn = dot(halfdir, n);
			
			float efisrt = -2.0 * ( (hdotx / roughnessx) * (hdotx / roughnessx) + (hdoty / roughnessy) * (hdoty / roughnessy) );
			float esecond = 1.0 + hdotn;
			float para1 = exp(efisrt / esecond);
			float para2 = 4.0 * PI * roughnessx * roughnessy * sqrt(NdotL * NdotV);
			color = (DiffColor + SpecColor *  (para1 / para2)) * max(0.0, NdotL) * LightColor;
			
		}
	}
	
	FragColor = vec4(color); 
}