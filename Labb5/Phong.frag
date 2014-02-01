#version 330 core
uniform sampler2D tex;

in vec3 interpolatedNormal;
in vec3 outPos;
in vec2 outTex;
in vec3 lightpos;

out vec4 color;


vec3 lightPos = vec3(1.0, 0.0, -1.0);
float gloss = 20.00;

void main()
{
	lightPos = lightpos;
	vec3 lightVec = normalize(lightPos-outPos);
	vec3 E = normalize(-outPos);
	vec3 R = reflect(-lightVec,interpolatedNormal);

	vec3 diffuse = clamp(texture(tex,outTex).rgb*max(dot(interpolatedNormal, lightVec) ,0.0), 0.0, 1.0);
	float spec = clamp( pow( max( dot(R,E),0.0 ),0.3*gloss), 0.0 , 1.0);
	vec3 specular = vec3(spec);
	color = vec4(diffuse+specular,1.0);
}