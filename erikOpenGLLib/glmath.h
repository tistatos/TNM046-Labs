/**
* @file glmath.h
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Math library for TNM046
*/

#include <xmmintrin.h>
#include <math.h>

const float PI = 3.14159265359f;
#define MATRIX __declspec(align(16))


// Matrix operations
void matrix4Mult(float m1[], float m2[], float mOut[]);
void matrix4RotX(float m[], float angle);
void matrix4RotY(float m[], float angle);
void matrix4RotZ(float m[], float angle);
void matrix4Eye(float m[]);

void matrix4Tran(float m[], float tranX, float tranY, float tranZ);

void matrix4Scale(float m[], float scaleX, float scaleY, float scaleZ);

inline float deg2rad(float angle) { return (angle*PI)/180;}
inline float rad2deg(float angle) {return (180*angle)/PI;}