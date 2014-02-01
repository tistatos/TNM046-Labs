/**
* @file main.cpp
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Labb 4 of TNM046
*/

#include "glmath.h"
#include "tnm046.h"
#include "triangleSoup.h"

void GLFWCALL SetupViewport(int width, int height)
{
	glfwGetWindowSize(&width, &height);
	glViewport(0, 0, width, height);
}

int main(int argc, char *argv[])
{
	GLint scene = 0;
	GLint location_MVP;
	GLint location_MV;
	GLdouble time;
	GLdouble lastTime=0;
	GLdouble dt;
	GLboolean running = GL_TRUE;

	GLuint programObject;
	glfwInit();

	if( !glfwOpenWindow( 512, 512, 8, 8, 8,8, 32 ,0, GLFW_WINDOW ))
	{
		glfwTerminate();
		return 1;
	}

	loadExtensions();

	printf("GL Vendor:			%s\n", glGetString(GL_VENDOR));
	printf("GL Render:			%s\n", glGetString(GL_RENDERER));
	printf("GL Version:			%s\n", glGetString(GL_VERSION));

	glfwSwapInterval(0);

	//No need to call setupviewport every frame, lets do this instead:
	glfwSetWindowSizeCallback(SetupViewport);

	programObject = createShader("phong.vert", "phong.frag");

#pragma region Sphere & Cube w. triangleSoup
	triangleSoup sphere;
	soupInit(&sphere);
	soupCreateSphere(&sphere,1.0f, 20);

	triangleSoup cube;
	soupInit(&cube);
	soupCreateBox(&cube, 1.0f, 1.0f, 1.0f);
#pragma endregion
#pragma region Matrix setup

	float fov = deg2rad(40.0f);
	float n = 0.50f;
	int f= 10.0f;
	int ratio = 1;

	fov =  1/tan(fov/2);

	MATRIX float M[16];
	
	MATRIX float V[] = {	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f, 		
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, -2.0f, 1.0f		};

	MATRIX float P[] = {	fov/ratio,	0.0f,	0.0f,			0.0f,
							0.0f,		fov,	0.0f,			0.0f, 
							0.0f,		0.0f,	(f+n)/(n-f),		-1.0f,
							0.0f,		0.0f,	(2.0f*f*n)/(n-f), 0.0f		};

	location_MVP = glGetUniformLocation(programObject, "MVP");
	location_MV = glGetUniformLocation(programObject, "MV");
#pragma endregion
	float zpos = 0;
	float xpos = 0;
	while(running)
	{
		time = glfwGetTime();
		dt = time-lastTime;

		if(glfwGetKey('W'))
			zpos += 2.0f*dt;
		if(glfwGetKey('S'))
			zpos -= 2.0f*dt;
		if(glfwGetKey('A'))
			xpos -= 2.0f*dt;
		if(glfwGetKey('D'))
			xpos += 2.0f*dt;
		
		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		matrix4Eye(M);
		matrix4Scale(M, 0.4f ,0.4f ,0.8f);
		matrix4RotY(M,time*40);
		matrix4Tran(M,xpos, 0.0f, zpos);

		MATRIX float MVP[16];
		MATRIX float MV[16];

		matrix4Mult(M, V, MV);
		matrix4Mult(MV, P, MVP);

		glUseProgram(programObject);
		if(location_MVP != -1)
			glUniformMatrix4fv(location_MVP,1,GL_FALSE,MVP);
		if(location_MV != -1)
			glUniformMatrix4fv(location_MV,1,GL_FALSE,MV);
		glEnable(GL_CULL_FACE);

		soupRender(cube);

		glfwSwapBuffers();
		lastTime = time;

		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			running = GL_FALSE;
	}
}