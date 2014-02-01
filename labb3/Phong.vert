#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

//vertex lighting
out vec3 interpolatedColor;

//fragment lighting
out vec3 interpolatedNormal;
out vec3 lightVec;

uniform mat4 C;

vec3 viewPos = vec3(0.0, 0.0, 1.0);
vec3 lightPos = vec3(9.0, 0.0, 0.0);
float gloss = 10.00;

void main()
{
	//vertex lighting
	//gl_Position = vec4(inPosition,1.0);
	//vec3 outPos = vec3(gl_Position);
	
	//vec4 lightPosTranslated = C*vec4(lightPos, 1.0);
	//lightPos = vec3(lightPosTranslated);
	//vec3 lightVec = normalize(lightPos-outPos);
	
	//float lightdotprod = dot(inNormal,lightVec);
	//vec3 halfangle = normalize(lightVec+viewPos); 
	//float specdot = max(dot(inNormal,halfangle),0);
	
	//specdot = pow(specdot,gloss);
	
	//vec3 specular = vec3(specdot,specdot,specdot);
	//vec3 diffuse = vec3(0.6,0.2,0.3)*lightdotprod;

	//interpolatedColor = diffuse+specular;

	//Fragment lighting
	gl_Position = vec4(inPosition,1.0);
	vec3 outPos = vec3(gl_Position);
	
	vec4 lightPosTranslated = C*vec4(lightPos, 1.0);
	lightPos = vec3(lightPosTranslated);
	lightVec = normalize(lightPos-outPos);
	interpolatedNormal = inNormal;
}