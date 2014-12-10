#version 400
layout(location=0) out vec4 FragColor;
const int maxKenekSize = 25;
float kenelValue[maxKenekSize];
uniform vec4 kenelValue0;
uniform vec4 kenelValue1;
uniform vec4 kenelValue2;
uniform int kernelSideSize;
uniform vec4 viewportSize;
uniform sampler2D scenetexture;
in vec2 tcoord;
void main()
{
	//temp method
		kenelValue[0] = kenelValue0.x;kenelValue[1] = kenelValue0.y;kenelValue[2] = kenelValue0.z;
		kenelValue[3] = kenelValue1.x;kenelValue[4] = kenelValue1.y;kenelValue[5] = kenelValue1.z;
		kenelValue[6] = kenelValue2.x;kenelValue[7] = kenelValue2.y;kenelValue[8] = kenelValue2.z;
	//
	vec4 sum = vec4(0.0);
	int sampRange = kernelSideSize / 2;
	int row = 0;
	int col = 0;
	int kernelSize = kernelSideSize * kernelSideSize;
	
	//for (int i = 0; i < kernelSize; ++i)
	//{
	//	sum += texture(scenetexture, tcoord + viewportSize.zw * vec2(row - sampRange, col - sampRange), 0);
	//	++col;
	//	row += col / kernelSideSize;
	//	col = int(mod(col, kernelSideSize));
	//}
	//vec4 color = sum / kernelSize;
	
//--------------------------------------------------------------------------------------------------------------

// a very simple 4x4 box filter
// the kernel has the following form
//   o o o o
//   o o o o
//   o o x o
//   o o o o 
// where x marks the fragment position and the o marks a sampling point

    for (int x = -2; x < 2; x++)
    for (int y = -2; y < 2; y++)
    {
        sum += texture(scenetexture, vec2(tcoord.x + x * viewportSize.z, tcoord.y + y * viewportSize.w));
    }
    sum /= 16;
    vec4 color = sum;
	
	FragColor = vec4(color);
}