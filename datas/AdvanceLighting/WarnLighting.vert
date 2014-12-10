#version 430
uniform mat4 u_ModelViewProjectMat;
uniform mat4 u_ModelViewMat;
uniform mat3 u_NormalMat;
uniform vec4 LightPos;
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
out vec3 eNormal;
out vec4 eVertex;
out vec4 eLightPos;
out vec4 eOrigin;

void main()
{
	eNormal = u_NormalMat * normal;
	eNormal = normalize(eNormal);
	eVertex = u_ModelViewMat * vec4(vertex, 1.0);
	eLightPos = u_ModelViewMat * LightPos;
	eOrigin = u_ModelViewMat * vec4(0.0, 0.0, 0.0, 1.0);
	gl_Position = u_ModelViewProjectMat * vec4(vertex, 1.0);
}