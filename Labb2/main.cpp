/**
* @file main.cpp
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Labb 2 of TNM046
*/

#include "tnm046.h"
#include "glmath.h"


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
	programObject = createShader("colorWMatrix.vert","color.frag");

#pragma region setup Cube
	GLuint cubeVertexArrayID;
	GLuint cubeVertexBufferID;
	GLuint cubeIndexBufferID;
	GLuint cubeColorBufferID;

	static const GLfloat cubeVertexDataArray[] =
	{
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f
	};

	static const GLfloat cubeColorDataArray[] =
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
	};

	static const GLint cubeIndexDataArray[] = 
	{
		//front
		2, 1, 0, 
		0, 3, 2, 
		//right
		6, 4, 1,
		2, 6, 1, 
		//back
		7, 5, 4,
		6, 7, 4,
		//left
		3, 0 ,5,
		7, 3, 5,
		//top
		6, 2, 3,
		7, 6, 3,
		//bottom
		1, 4, 5,
		0, 1, 5 
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

#pragma region Setup Triangle

	//Declare identifiers
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint colorBufferID;

	//List of our Vertices
	static const GLfloat vertexDataArray[] = 
	{ 
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	static const GLfloat colorDataArray[] = 
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	//Index array
	static const GLuint indexArrayData[] = { 0,1,2 };

	//Generate a vertexArray and store identifier
	glGenVertexArrays(1, &vertexArrayID);

	//Generate buffers and store identifiers
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &indexBufferID);
	glGenBuffers(1, &colorBufferID);

	//Bind vertexArray
	glBindVertexArray(vertexArrayID);
	//bind vertex buffer and put our data into the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexDataArray), vertexDataArray, GL_STATIC_DRAW);

	//Tell shaders what data is in this attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

	//Bind index buffer and insert our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayData), indexArrayData, GL_STATIC_DRAW);

	//Bind color buffer 
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorDataArray), colorDataArray, GL_STATIC_DRAW);

	//Tell shaders what data is in this attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

#pragma endregion

#pragma region Matrix setup
	
	//Translation
	//__declspec(align(16)) float T[16] =
	//{
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.3f, 0.3f, 0.0f, 1.0f
	//};

	//Scaling
	//GLfloat T[16] =
	//{
	//	0.2f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 0.2f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//};
	//
	////.2 scaling, pi/4 rotation, translate .2 units (T*R*S)
	//GLfloat T[16] =
	//{
	//	0.14f, -0.14f, 0.0f, 0.0f,
	//	0.14f, 0.14f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.2f, 0.2f, 0.0f, 1.0f
	//};

	//rotation by pi/4
	//__declspec(align(16)) float R[16] =
	//{
	//	0.70f, -0.70f, 0.0f, 0.0f,
	//	0.70f, 0.70f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//};
	MATRIX float I[16];

	MATRIX float C[16];
	

	location_C = glGetUniformLocation(programObject, "C");
#pragma endregion

#pragma region Rendering Loop
	while(running)
	{
		computeFPS();
		matrix4Eye(C);
		matrix4Scale(C, 0.4f ,0.4f ,0.4f);
		glEnable(GL_CULL_FACE);
		time = glfwGetTime();
		dt = time-lastTime;

		if(glfwGetKey('1'))
			scene = 0;
		else if(glfwGetKey('2'))
			scene = 1;
		else if(glfwGetKey('3'))
			scene = 2;

		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// calculate our rotation
		double rotationSpeed = 45*time;
		if(scene ==0)
		{
			matrix4RotY(C,rotationSpeed);
			glUseProgram(programObject);
			if(location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glBindVertexArray(vertexArrayID);
			//glEnable(GL_CULL_FACE);
			//glPolygonMode(GL_BACK,GL_LINE);
			
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0 );
		}
		else if(scene == 1)
		{
			matrix4RotY(C,rotationSpeed);
			matrix4RotX(C,20);
			glUseProgram(programObject);
			if(location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glBindVertexArray(cubeVertexArrayID);
			glPolygonMode(GL_BACK,GL_LINE);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		else
		{
			matrix4RotY(C,rotationSpeed*4);
			matrix4Tran(C, 0.8f, 0.0f, 0.0f);
			matrix4RotY(C,rotationSpeed*2);
			matrix4RotX(C,15);
			glUseProgram(programObject);
			if(location_C != -1)
				glUniformMatrix4fv(location_C,1,GL_FALSE,C);
			glBindVertexArray(cubeVertexArrayID);
			glPolygonMode(GL_BACK,GL_LINE);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		
		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers();
		lastTime = time;
		
		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			running = GL_FALSE;
	}
#pragma endregion

	glfwTerminate();
	return 0;
}