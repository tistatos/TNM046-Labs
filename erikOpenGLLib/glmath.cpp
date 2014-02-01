/**
* @file glmath.cpp
* @author Erik Larsson
* @version 1.0
* @section DESCRIPTION
* Math library for TNM046
*/

#include "glmath.h"

void matrix4Eye(float m[])
{
	m[0] = 1.0f; m[1] = 0.0f; m[2] =0.0f; m[3] = 0.0f;
	m[4] = 0.0f; m[5] = 1.0f; m[6] =0.0f; m[7] = 0.0f;
	m[8] = 0.0f; m[9] = 0.0f; m[10] =1.0f; m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] =0.0f; m[15] = 1.0f;
}

void matrix4Mult(float m1[], float m2[], float mOut[])
{
#ifdef _USE_SIMD_
	/* How it works: SIMD (Single Instruction Multiple Data) uses part of the CPU make quick vector operations
		http://download.intel.com/design/PentiumIII/sml/24504501.pdf
		_mm_set_ps1(x) - creates a 1x4 vector with all components set to x.
		_mm_load_ps(x) - load 32-bit floats from x, 16bit alignment needed!.
		_mm_mul_ps(x,y) - multiply vectors x,y component wise {x_1*y_1,x_2*y_2,x_3*y_3,x_4*y_4}
		_mm_add_ps(x,y) - add vectors x,y component wise.
		_mm_store_ps(a,x) store vector at memory address a.
	*/
	__m128 t1, t2;
	__m128 a = _mm_load_ps(m2);
	__m128 b = _mm_load_ps(m2+4);
	__m128 c = _mm_load_ps(m2+8);
	__m128 d = _mm_load_ps(m2+12);

   	t1 = _mm_set_ps1(m1[0]);
	t2 = _mm_mul_ps(t1,a);
	t1 = _mm_set_ps1(m1[1]);
	t2 = _mm_add_ps(_mm_mul_ps(b,t1),t2);
	t1 = _mm_set_ps1(m1[2]);
	t2 = _mm_add_ps(_mm_mul_ps(c,t1),t2);
	t1 = _mm_set_ps1(m1[3]);
	t2 = _mm_add_ps(_mm_mul_ps(d,t1),t2);
	_mm_store_ps(mOut, t2);

	t1 = _mm_set_ps1(m1[4]);
	t2 = _mm_mul_ps(t1,a);
	t1 = _mm_set_ps1(m1[5]);
	t2 = _mm_add_ps(_mm_mul_ps(b,t1),t2);
	t1 = _mm_set_ps1(m1[6]);
	t2 = _mm_add_ps(_mm_mul_ps(c,t1),t2);
	t1 = _mm_set_ps1(m1[7]);
	t2 = _mm_add_ps(_mm_mul_ps(d,t1),t2);
	_mm_store_ps(mOut+4, t2);

	t1 = _mm_set_ps1(m1[8]);
	t2 = _mm_mul_ps(t1,a);
	t1 = _mm_set_ps1(m1[9]);
	t2 = _mm_add_ps(_mm_mul_ps(b,t1),t2);
	t1 = _mm_set_ps1(m1[10]);
	t2 = _mm_add_ps(_mm_mul_ps(c,t1),t2);
	t1 = _mm_set_ps1(m1[11]);
	t2 = _mm_add_ps(_mm_mul_ps(d,t1),t2);
	_mm_store_ps(mOut+8, t2);

	t1 = _mm_set_ps1(m1[12]);
	t2 = _mm_mul_ps(t1,a);
	t1 = _mm_set_ps1(m1[13]);
	t2 = _mm_add_ps(_mm_mul_ps(b,t1),t2);
	t1 = _mm_set_ps1(m1[14]);
	t2 = _mm_add_ps(_mm_mul_ps(c,t1),t2);
	t1 = _mm_set_ps1(m1[15]);
	t2 = _mm_add_ps(_mm_mul_ps(d,t1),t2);
	_mm_store_ps(mOut+12, t2);

#else
	mOut[0]		= m1[0]*m2[0] + 	m1[1]*m2[4] + 	m1[2]*m2[8] + 	m1[3]*m2[12];
	mOut[1]		= m1[0]*m2[1] + 	m1[1]*m2[5] + 	m1[2]*m2[9] + 	m1[3]*m2[13];
	mOut[2]		= m1[0]*m2[2] + 	m1[1]*m2[6] + 	m1[2]*m2[10] + 	m1[3]*m2[14];
	mOut[3]		= m1[0]*m2[3] + 	m1[1]*m2[7] + 	m1[2]*m2[11] + 	m1[3]*m2[15];
	mOut[4]		= m1[4]*m2[0] + 	m1[5]*m2[4] + 	m1[6]*m2[8] + 	m1[7]*m2[12];
	mOut[5]		= m1[4]*m2[1] + 	m1[5]*m2[5] + 	m1[6]*m2[9] + 	m1[7]*m2[13];
	mOut[6]		= m1[4]*m2[2] + 	m1[5]*m2[6] + 	m1[6]*m2[10] + 	m1[7]*m2[14];
	mOut[7]		= m1[4]*m2[3] + 	m1[5]*m2[7] + 	m1[6]*m2[11] + 	m1[7]*m2[15];
	mOut[8]		= m1[8]*m2[0] + 	m1[9]*m2[4] + 	m1[10]*m2[8] + 	m1[11]*m2[12];
	mOut[9]		= m1[8]*m2[1] + 	m1[9]*m2[5] + 	m1[10]*m2[9] + 	m1[11]*m2[13];
	mOut[10]	= m1[8]*m2[2] + 	m1[9]*m2[6] + 	m1[10]*m2[10] + m1[11]*m2[14];
	mOut[11]	= m1[8]*m2[3] + 	m1[9]*m2[7] + 	m1[10]*m2[11] + m1[11]*m2[15];
	mOut[12]	= m1[12]*m2[0] + 	m1[13]*m2[4] + 	m1[14]*m2[8] + 	m1[15]*m2[12];
	mOut[13]	= m1[12]*m2[1] + 	m1[13]*m2[5] + 	m1[14]*m2[9] + 	m1[15]*m2[13];
	mOut[14]	= m1[12]*m2[2] + 	m1[13]*m2[6] + 	m1[14]*m2[10] + m1[15]*m2[14];
	mOut[15]	= m1[12]*m2[3] + 	m1[13]*m2[7] + 	m1[14]*m2[11] + m1[15]*m2[15];
#endif
}

void matrix4RotX(float m[], float angle)
{
	MATRIX float r[16];
	matrix4Eye(r);
	float rad = deg2rad(angle);
	float cosAngle = cos(rad);
	float sinAngle = sin(rad);
	
	r[5] = cosAngle;
	r[6] = sinAngle;
	r[9] = -sinAngle;
	r[10] = cosAngle;

	matrix4Mult(m,r,m);
}

void matrix4RotY(float m[], float angle)
{
	MATRIX float r[16];
	matrix4Eye(r);

	float rad = deg2rad(angle);
	float cosAngle = cos(rad);
	float sinAngle = sin(rad);
	
	r[0] = cosAngle;
	r[2] = sinAngle;
	r[8] = -sinAngle;
	r[10] = cosAngle;

	matrix4Mult(m,r,m);
}

void matrix4RotZ(float m[], float angle)
{
	MATRIX float r[16];
	matrix4Eye(r);

	float rad = deg2rad(angle);
	float cosAngle = cos(rad);
	float sinAngle = sin(rad);

	r[0] = cosAngle;
	r[1] = sinAngle;
	r[4] = -sinAngle;
	r[5] = cosAngle;

	matrix4Mult(m,r,m);
}

void matrix4Tran(float m[], float tranX, float tranY, float tranZ)
{
	MATRIX float t[16];
	matrix4Eye(t);
	t[12] = tranX;
	t[13] = tranY;
	t[14] = tranZ;

	matrix4Mult(m,t,m);
}

void matrix4Scale(float m[], float scaleX, float scaleY, float scaleZ)
{
	MATRIX float s[16];
	matrix4Eye(s);
	s[0] = scaleX;
	s[5] = scaleY;
	s[10] = scaleZ;

	matrix4Mult(m,s,m);
}