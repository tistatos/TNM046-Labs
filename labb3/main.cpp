/**
* @file main.cpp
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Labb 3 of TNM046
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
	GLint location_C;
	GLdouble time;
	GLdouble lastTime=0;
	GLdouble dt;
	GLboolean running = GL_TRUE;
	GLuint programObject;
	GLuint programObjectWNormal;
	GLuint programObjectPhong;
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

	programObject = createShader("colorWMatrix.vert", "color.frag");
	programObjectWNormal = createShader("colorWMatrixWNormals.vert", "color.frag");
	//programObjectPhong = createShader("phong.vert", "color.frag");
	programObjectPhong = createShader("phong.vert", "phong.frag");
	
#pragma region setup Cube
	GLuint cubeVertexArrayID;
	GLuint cubeVertexBufferID;
	GLuint cubeIndexBufferID;
	GLuint cubeColorBufferID;

	static const GLfloat cubeVertexDataArray[] =
	{
		-1.0f, -1.0f, -1.0f, //0
		1.0f, -1.0f, -1.0f, //1
		1.0f, 1.0f, -1.0f, //2

		1.0f, 1.0f, -1.0f, //2
		-1.0f, 1.0f, -1.0f, //3
		-1.0f, -1.0f, -1.0f, //0
		
		1.0f, -1.0f, -1.0f, //1
		1.0f, -1.0f, 1.0f, //4
		1.0f, 1.0f, 1.0f, //6

		1.0f, -1.0f, -1.0f, //1
		1.0f, 1.0f, 1.0f, //6
		1.0f, 1.0f, -1.0f, //2

		1.0f, -1.0f, 1.0f, //4
		-1.0f, -1.0f, 1.0f, //5
		-1.0f, 1.0f, 1.0f, //7

		1.0f, -1.0f, 1.0f, //4
		-1.0f, 1.0f, 1.0f, //7
		1.0f, 1.0f, 1.0f, //6

		-1.0f, -1.0f, 1.0f, //5
		-1.0f, -1.0f, -1.0f, //0
		-1.0f, 1.0f, -1.0f, //3

		-1.0f, -1.0f, 1.0f, //5
		-1.0f, 1.0f, -1.0f, //3
		-1.0f, 1.0f, 1.0f, //7

		-1.0f, 1.0f, -1.0f, //3
		1.0f, 1.0f, -1.0f, //2
		1.0f, 1.0f, 1.0f, //6

		-1.0f, 1.0f, -1.0f, //3
		1.0f, 1.0f, 1.0f, //6
		-1.0f, 1.0f, 1.0f, //7

		-1.0f, -1.0f, 1.0f, //5
		1.0f, -1.0f, 1.0f, //4
		1.0f, -1.0f, -1.0f, //1

		-1.0f, -1.0f, 1.0f, //5
		1.0f, -1.0f, -1.0f, //1
		-1.0f, -1.0f, -1.0f, //0
	};

	static const GLfloat cubeColorDataArray[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

	};

	static const GLint cubeIndexDataArray[] = 
	{
		0, 1, 2,
		3, 4, 5, 
		
		6, 7, 8, 
		9, 10, 11, 
		
		12, 13, 14,
		15, 16, 17, 
		
		18, 19, 20, 
		21, 22, 23,

		24, 25, 26,
		27, 28, 29,
		
		30, 31, 32,
		33, 34, 35
	};

	glGenVertexArrays(1, &cubeVertexArrayID);
	glGenBuffers(1,&cubeVertexBufferID);
	glGenBuffers(1,&cubeColorBufferID);
	glGenBuffers(1,&cubeIndexBufferID);

	glBindVertexArray(cubeVertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertexDataArray),cubeVertexDataArray, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeColorBufferID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeColorDataArray),cubeColorDataArray, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexDataArray),cubeIndexDataArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
#pragma endregion
#pragma region Sphere w. triangleSoup
	triangleSoup sphere;
	triangleSoup cube;
	soupInit(&sphere);
	soupInit(&cube);
	soupCreateBox(&cube, 0.7f, 0.7f, 0.7f);
	soupCreateSphere(&sphere,1.0f, 20);
#pragma endregion
#pragma region Matrix setup

	MATRIX float I[16];
	MATRIX float C[16];

	matrix4Eye(I);
	matrix4Eye(C);
	
	location_C = glGetUniformLocation(programObject, "C");
#pragma endregion
	while(running)
	{
		time = glfwGetTime();
		dt = time-lastTime;

		if(glfwGetKey('1'))
			scene = 0;
		else if(glfwGetKey('2'))
			scene = 1;
		else if(glfwGetKey('3'))
			scene = 2;
		else if(glfwGetKey('4'))
			scene = 3;

		double rotationSpeed = 45*time;
		matrix4Eye(C);
		matrix4Scale(C, 0.4f ,0.4f ,0.4f);

		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		if(scene ==0)
		{
			matrix4RotX(C,30);
			matrix4RotY(C,rotationSpeed);

			glUseProgram(programObject);
			if(location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glBindVertexArray(cubeVertexArrayID);
			glPolygonMode(GL_FRONT,GL_FILL);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		else if(scene == 1)
		{
			
			matrix4RotX(C,90);
			matrix4RotY(C,rotationSpeed);

			glUseProgram(programObject);
			if(location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glPolygonMode(GL_FRONT,GL_FILL);
			glPolygonMode(GL_BACK,GL_POINT);
			soupRender(sphere);
		}
		else if( scene == 2)
		{
			matrix4RotY(C,rotationSpeed);

			glUseProgram(programObjectWNormal);
			if (location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glPolygonMode(GL_FRONT,GL_FILL);
			glPolygonMode(GL_BACK,GL_POINT);
			soupRender(sphere);
		}
		else if(scene == 3)
		{
			matrix4RotY(C,rotationSpeed);

			glUseProgram(programObjectPhong);
			if (location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glPolygonMode(GL_FRONT,GL_FILL);
			soupRender(sphere);
		}

		glfwSwapBuffers();
		lastTime = time;

		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			running = GL_FALSE;
	}
}