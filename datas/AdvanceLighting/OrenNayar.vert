#version 400
uniform mat4 ogre_ModelViewProjectMatrix;
uniform mat4 ogre_ModelViewMatrix;
uniform mat4 ogre_NormalMatrix;
uniform vec4 LightPos;
in vec4 vertex;
in vec3 normal;
out vec3 eNormal;
out vec4 eVertex;
out vec4 eLightPos;

void main()
{
	eNormal = (ogre_NormalMatrix * vec4(normal, 0.0)).xyz;
	eVertex = ogre_ModelViewMatrix * vertex;
	eLightPos = ogre_ModelViewMatrix * LightPos;
	gl_Position = ogre_ModelViewProjectMatrix * vertex;
}