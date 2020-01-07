
#include "IkeWorld.h"

// get the magnitude of a given vector

double vecMagnitude3(Vector3* vector){
	double x,y,z;
	x = vector->x;
	y = vector->y;
	z = vector->z;
	return sqrt(x*x+y*y+z*z);
}

double vecMagnitude2(Vector2* vector){
	double x,y;
	x = vector->x;
	y = vector->y;
	return sqrt(x*x+y*y);
}

// normalize a given vector

Vector3* vecNormalize3(Vector3* vector){
	// these functions are designed to be used together in one line like this:
	return vecDiv3(vector, vecMagnitude3(vector));
}

Vector2* vecNormalize2(Vector2* vector){
	return vecDiv2(vector, vecMagnitude2(vector));
}

// add two vectors: vector a = vector a + vector b
// returns vector a

Vector3* vecAdd3(Vector3* a, Vector3* b){
	a->x+=b->x;
	a->y+=b->y;
	a->z+=b->z;
	return a;
}
Vector2* vecAdd2(Vector2* a, Vector2* b){
	a->x+=b->x;
	a->y+=b->y;
	return a;
}

// subtract two vectors: vector a = vector a - vector b
// returns vector a

Vector3* vecSub3(Vector3* a, Vector3* b){
	a->x-=b->x;
	a->y-=b->y;
	a->z-=b->z;
	return a;
}
Vector2* vecSub2(Vector2* a, Vector2* b){
	a->x-=b->x;
	a->y-=b->y;
	return a;
}

// negitate a vector: vector = -vector
// returns vector

Vector3* vecNeg3(Vector3* vector){
	vector->x=-vector->x;
	vector->y=-vector->y;
	vector->z=-vector->z;
	return vector;
}
Vector2* vecNeg2(Vector2* vector){
	vector->x=-vector->x;
	vector->y=-vector->y;
	return vector;
}

// multiply a vector by a scalar
// returns vector

Vector3* vecMul3(Vector3* vector, double val){
	vector->x*=val;
	vector->y*=val;
	vector->z*=val;
	return vector;
}
Vector2* vecMul2(Vector2* vector, double val){
	vector->x*=val;
	vector->y*=val;
	return vector;
}

// divide a vector by a scalar
// returns vector

Vector3* vecDiv3(Vector3* vector, double val){
	vector->x/=val;
	vector->y/=val;
	vector->z/=val;
	return vector;
}
Vector2* vecDiv2(Vector2* vector, double val){
	vector->x/=val;
	vector->y/=val;
	return vector;
}
