#version 400
uniform mat4 ogre_ModelViewMatrix;
uniform mat4 ogre_NormalMatrix;
uniform mat4 ogre_ModelViewProjectMatrix;
uniform float farDis;
in vec4 vertex;
in vec3 normal;
out vec4 ePosition;
out vec3 eNormal;
out float zLinerDepth;
void main()
{
	ePosition = ogre_ModelViewMatrix * vertex;
	eNormal = (ogre_NormalMatrix * vec4(normal, 0.0)).xyz;
	eNormal = normalize(eNormal);
	gl_Position = ogre_ModelViewProjectMatrix * vertex;
	zLinerDepth = gl_Position.w / farDis;
}
