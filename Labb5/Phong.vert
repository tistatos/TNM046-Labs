#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTex;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 light;

out vec3 interpolatedNormal;
out vec3 outPos;
out vec2 outTex;
out vec3 lightpos;

void main()
{
	gl_Position = vec4(MVP*vec4(inPosition,1.0));
	outPos = vec3(MV*vec4(inPosition,1.0));
	interpolatedNormal = normalize(vec3(transpose(inverse(MV))*vec4(inNormal,1.0)));
	outTex = inTex;
	lightpos = vec3(light[3]);
}
