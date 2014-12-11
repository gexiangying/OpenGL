#version 400
uniform mat4 u_ModelViewProjectMat;
uniform mat4 u_ModelViewMat;
uniform mat3 u_NormalMat;
uniform vec4 LightPos;
layout(location=0) in vec4 vertex;
layout(location=1) in vec3 normal;
out vec3 eNormal;
out vec4 eVertex;
out vec4 eLightPos;

void main()
{
	eNormal = u_NormalMat * normal;
	eVertex = u_ModelViewMat * vertex;
	eLightPos = LightPos;
	gl_Position = u_ModelViewProjectMat * vertex;
}