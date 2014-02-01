#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 C;
vec3 lightPos = vec3(9.0, 1.0, 0.0);

out vec3 interpolatedColor;

void main()
{
	//rotating Sphere
	//gl_Position = C*vec4(inPosition,1.0);
	//vec3 outPos = vec3(gl_Position);
	//mat3 T = mat3(C);
	//vec3 transNormal = normalize(T*inNormal);
	//vec3 lightVec = normalize(lightPos-outPos);
	//float dotprod = dot(transNormal,lightVec);
	//interpolatedColor = vec3(dotprod,dotprod,dotprod);

	//rotating light
	gl_Position = vec4(inPosition,1.0);
	vec3 outPos = vec3(gl_Position);
	vec4 lightPosTranslated = C*vec4(lightPos, 1.0);
	lightPos = vec3(lightPosTranslated);
	vec3 lightVec = normalize(lightPos-outPos);
	float dotprod = dot(inNormal,lightVec);
	interpolatedColor = vec3(dotprod,dotprod,dotprod);	
}