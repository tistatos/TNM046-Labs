/**
* @file main.cpp
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Labb 5 of TNM046
*/

#include "glmath.h"
#include "pollRotator.h"
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
	GLint location_tex;
	GLint location_light;

	GLint location_cMVP;
	GLint location_cMV;

	GLdouble time;
	GLdouble lastTime=0;
	GLdouble dt;
	
	GLboolean running = GL_TRUE;

	GLuint programObject;
	GLuint colorProgram;
	GLuint textureID;

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
	colorProgram = createShader("phong.vert", "phongcolor.frag");

#pragma region Sphere w. triangleSoup
	triangleSoup sphere;
	triangleSoup model;
	soupInit(&model);
	soupReadOBJ(&model,"trex.obj");
	
	soupInit(&sphere);
	soupCreateSphere(&sphere,1.0f, 20);

	triangleSoup cube;
	soupInit(&cube);
	soupCreateBox(&cube, 1.0f, 1.0f, 1.0f);

#pragma endregion

#pragma region Matrix setup

	float fov = 1.0f/tan(deg2rad(40.0f));
	float n = 0.5f;
	int f= 20.0f;
	float ratio = 1.0f;


	MATRIX float M[16];
	MATRIX float V[] = {	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f, 		
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f		};
	
	MATRIX float P[] = {	fov/ratio,	0.0f,	0.0f,			0.0f,
							0.0f,		fov,	0.0f,			0.0f, 
							0.0f,		0.0f,	(f+n)/(n-f),		-1.0f,
							0.0f,		0.0f,	(2.0f*f*n)/(n-f), 0.0f		};

	location_MVP = glGetUniformLocation(programObject, "MVP");
	location_MV = glGetUniformLocation(programObject, "MV");
	location_light = glGetUniformLocation(programObject, "light");

	location_cMVP = glGetUniformLocation(colorProgram, "MVP");
	location_cMV = glGetUniformLocation(colorProgram, "MV");
#pragma endregion
#pragma region Texture setup
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int trex = glfwLoadTexture2D("trex.tga",GLFW_BUILD_MIPMAPS_BIT);

	location_tex = glGetUniformLocation(programObject, "tex");
#pragma endregion

#pragma region controls
	rotatorMouse mouse;
	initRotatorMouse(&mouse);
	rotatorKey board;
	initRotatorKey(&board);
#pragma endregion

	glEnable (GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	while(running)
	{
		computeFPS();
		time = glfwGetTime();
		dt = time-lastTime;

		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MATRIX float MVP[16];
		MATRIX float MV[16];

		pollRotatorMouse(&mouse);
		pollRotatorKey(&board);
		
		matrix4Eye(V);
		matrix4RotY(V,mouse.phi);
		matrix4RotX(V,mouse.theta);
		matrix4Tran(V,0.0f,0.0f,-2.0f);

#pragma region sphere
		matrix4Eye(M);
		matrix4Scale(M,0.1f,0.1f,0.1f);
		matrix4Tran(M,1.0f,0.0f,0.0f);
		matrix4RotY(M,board.phi);
		matrix4RotX(M,board.theta);

		matrix4Mult(M,V,MV);
		matrix4Mult(MV,P,MVP);

		glUseProgram(colorProgram);
		if(location_MVP != -1)
			glUniformMatrix4fv(location_cMVP,1,GL_FALSE,MVP);
		if(location_MV != -1)
			glUniformMatrix4fv(location_cMV,1,GL_FALSE,MV);

		soupRender(sphere);
#pragma endregion

#pragma region Trex
		glUseProgram(programObject);
		//send light position to shader
		if(location_light != -1)
			glUniformMatrix4fv(location_light,1,GL_FALSE,MV);

		matrix4Eye(M);

		matrix4Mult(M,V,MV);
		matrix4Mult(MV,P,MVP);

		glBindTexture(GL_TEXTURE_2D,textureID);
		glUniform1i(location_tex, 0);

		if(location_MVP != -1)
			glUniformMatrix4fv(location_MVP,1,GL_FALSE,MVP);
		if(location_MV != -1)
			glUniformMatrix4fv(location_MV,1,GL_FALSE,MV);

		soupRender(model);
		glBindTexture(GL_TEXTURE_2D, 0);

#pragma endregion

		glUseProgram(0);
		glfwSwapBuffers();
		lastTime = time;

		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			running = GL_FALSE;
	}
}