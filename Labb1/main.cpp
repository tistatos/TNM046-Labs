/**
* @file main.cpp
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Labb 1 of TNM046
*/
#include "tnm046.h"

void GLFWCALL SetupViewport(int width, int height)
{
	glfwGetWindowSize(&width, &height);
	glViewport(0, 0, width, height);
}

int main(int argc, char *argv[])
{
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
	programObject = createShader("color.vert","color.frag");

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

#pragma region Rendering Loop
	while(running)
	{
		computeFPS();

		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_CLEAR_VALUE);

		glUseProgram(programObject);
		glBindVertexArray(vertexArrayID);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0 );

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers();
		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			running = GL_FALSE;
	}
#pragma endregion
	glfwTerminate();
	return 0;
}