#version 430                        
uniform vec4 u_diffuseColor;        
uniform vec4 u_specularColor;       
uniform vec4 u_ambientColor;        
uniform float u_shininess;          
uniform sampler2D u_diffuseTex;     
in vec3 v_position;                 
in vec3 v_normal;                   
in vec2 v_texcoord;  
in float zLinerDepth;    
layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 fragPosition;
layout(location=2) out vec4 fragNormal;
out vec4 color;                     
void main(){                        
	vec3 lDir = normalize(vec3(1.0, 1.0, 1.0));          
	vec3 normal = normalize(v_normal);                   
	vec3 eye = normalize(-v_position);                   
	vec3 H = normalize(eye + lDir);                      
	float specfactor = pow(max(0.0, dot(normal, H)), u_shininess);      
	float diffactor = max(0.0, dot(normal, lDir));                      
	fragColor = texture(u_diffuseTex, v_texcoord) * diffactor * u_diffuseColor + specfactor * u_specularColor; 
	fragPosition = vec4(v_position, 1.0);
	fragNormal = vec4(v_normal, zLinerDepth);
}