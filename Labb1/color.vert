#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

out vec3 interpolatedColor;

void main()
{
	gl_Position = vec4(0.4*inPosition,1.0);
	interpolatedColor = inColor;
}