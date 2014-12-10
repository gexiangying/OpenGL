#version 400
uniform mat4 ogre_ModelViewProjectMatrix;
in vec4 vertex;
in vec4 uv0;
out vec2 tcoord;
void main()
{
	tcoord = uv0.st;
	gl_Position = ogre_ModelViewProjectMatrix * vertex;
}
