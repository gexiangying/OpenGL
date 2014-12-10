#version 400
layout(location=0) out vec4 VPosition;
layout(location=1) out vec4 VNormal;
in vec4 ePosition;
in vec3 eNormal;
in float zLinerDepth;
void main()
{
	VPosition = ePosition;
	VNormal = vec4(eNormal, zLinerDepth);
}