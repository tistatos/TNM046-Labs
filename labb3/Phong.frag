#version 330 core

in vec3 interpolatedNormal;
in vec3 lightVec;
out vec4 color;

vec3 viewPos = vec3(0.0, 0.0, 1.0);
float gloss = 10.00;

void main()
{
	float lightdotprod = dot(interpolatedNormal,lightVec);
	vec3 halfAngle = normalize(lightVec+viewPos);
	float specdot = max(dot(interpolatedNormal,halfAngle),0);
	specdot = pow(specdot,gloss);

	vec3 specular = vec3(specdot,specdot,specdot);
	vec3 diffuse = vec3(0.6,0.2,0.3)*lightdotprod;
	
	color = vec4(diffuse + specular,1.0);
}