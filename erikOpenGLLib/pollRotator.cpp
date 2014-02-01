#include <GL/glfw.h>
#include <math.h>

#include "pollRotator.h"

void initRotatorKey(rotatorKey *state) {
     state->phi = 0.0;
     state->theta = 0.0;
};

void pollRotatorKey(rotatorKey *state) {

	double thisTime, elapsedTime;

	thisTime = glfwGetTime();
	elapsedTime = thisTime - state->lastTime;
	state->lastTime = thisTime;

	if(glfwGetKey(GLFW_KEY_RIGHT)) {
		state->phi += elapsedTime*90.0; // Rotate 90 degrees per second
		state->phi = fmod(state->phi, 360.0f); // Wrap around at 360.0
	}

	if(glfwGetKey(GLFW_KEY_LEFT)) {
		state->phi -= elapsedTime*90.0; // Rotate 90 degrees per second
		state->phi = fmod(state->phi, 360.0f);
		if (state->phi < 0.0) state->phi += 360.0; // If phi<0, then fmod(phi,360)<0
	}

	if(glfwGetKey(GLFW_KEY_UP)) {
		state->theta += elapsedTime*90.0; // Rotate 90 degrees per second
		if (state->theta >= 90.0) state->theta = 90.0; // Clamp at 90
	}

	if(glfwGetKey(GLFW_KEY_DOWN)) {
		state->theta -= elapsedTime*90.0; // Rotate 90 degrees per second
		if (state->theta < -90.0) state->theta = -90.0;      // Clamp at -90
	}
}


void initRotatorMouse(rotatorMouse *state) {
     state->phi = 0.0;
     state->theta = 0.0;
}

void pollRotatorMouse(rotatorMouse *state) {
  
  int thisX;
  int thisY;
  int thisLeft;
  int thisRight;
  int moveX;
  int moveY;
  int windowWidth;
  int windowHeight;

  // Find out where the mouse pointer is, and which buttons are pressed
  glfwGetMousePos(&thisX, &thisY);
  thisLeft = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
  thisRight = glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
  glfwGetWindowSize( &windowWidth, &windowHeight );

  if(thisLeft && state->lastLeft) { // If a left button drag is in progress
    moveX = thisX - state->lastX;
    moveY = thisY - state->lastY;
  	state->phi += 180.0 * moveX/windowWidth; // Longest drag rotates 180 degrees
	if (state->phi >= 360.0) state->phi = fmod(state->phi, 360.0f);
	if (state->phi < 0.0) state->phi += 360.0; // If phi<0, then fmod(phi,360)<0
  	state->theta += 180.0 * moveY/windowHeight; // Longest drag rotates 180 deg
	if (state->theta >= 90.0) state->theta = 90.0; // Clamp at 90
	if (state->theta < -90.0) state->theta = -90.0;      // Clam at -90
  }
  state->lastLeft = thisLeft;
  state->lastRight = thisRight;
  state->lastX = thisX;
  state->lastY = thisY;  
}
