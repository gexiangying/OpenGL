#version 430
layout(location=0) out vec4 FragColor;
uniform sampler2D u_Texture0;//postexture;
uniform sampler2D u_Texture1;//normtexture;
uniform sampler3D u_Texture2;//offsettexture;
uniform vec3 OffsetTexSize;
uniform float aoDensity;
uniform float sampleDepthViewSpace;
uniform float sampleLengthViewSpace;
uniform float edgeHighlight = 1.0;
uniform float defaultAccessibility = 0.5;
in vec2 tcoord;

void main()
{
	//vec4 pos = texture(u_Texture0, tcoord);
	vec4 n_d = texture(u_Texture1, tcoord);
	vec3 norm = n_d.xyz;
	//if (dot(norm, norm) <= 0.0) discard;
	float depth = n_d.w;
	
	ivec3 offsetCoord;
	offsetCoord.xy = ivec2(mod(gl_FragCoord.xy, OffsetTexSize.xy));
	float accessibility = 0;
	vec2 curCoord = tcoord;
	float depthP = depth;
	
	for (int i = 0; i < int(OffsetTexSize.z); ++i)
	{
		offsetCoord.z = i;
		vec3 value = texelFetch(u_Texture2, offsetCoord, 0).xyz;
		if (dot(value, norm) < 0.0)
			value = -value;
		vec3 offsets = value * vec3(aoDensity, aoDensity, sampleDepthViewSpace);
			
		vec3 posS = vec3(curCoord, depthP) + offsets;
		float depthS = texture(u_Texture1, posS.xy).w;
			
		float frangeIsInvalid = clamp((depthP - depthS) * sampleLengthViewSpace, 0.0, 1.0);
			
		if (posS.z < depthS)
			accessibility += mix(1.0, defaultAccessibility, frangeIsInvalid);
		else
			accessibility += mix(defaultAccessibility, 0.0, frangeIsInvalid);
	}
	
	float visibility = accessibility / OffsetTexSize.z * edgeHighlight;
	FragColor = vec4(visibility, visibility, visibility, 1.0); 
}