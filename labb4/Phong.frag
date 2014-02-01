#version 330 core

in vec3 interpolatedNormal;
in vec3 outPos;
out vec4 color;

vec3 lightPos = vec3(0.5, 0.0, -1.0);
float gloss = 5.00;

void main()
{
	vec3 lightVec = normalize(-lightPos);
	vec3 E = normalize(-outPos);
	vec3 R = reflect(-lightVec,interpolatedNormal);

	vec3 diffuse = clamp(vec3(0.7,0.3,0.3)*max(dot(interpolatedNormal, lightVec) ,0.0), 0.0, 1.0);
	float spec = clamp( pow( max( dot(R,E),0.0 ),0.3*gloss), 0.0 , 1.0);
	vec3 specular = vec3(spec);
	color = vec4(diffuse+specular,1.0);
}