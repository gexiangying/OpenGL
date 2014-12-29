#version 430
uniform mat4 u_ModelViewProjectMat;
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 uv;
out vec2 tcoord;
void main()
{
	tcoord = uv;
	gl_Position = u_ModelViewProjectMat * vertex;
}
