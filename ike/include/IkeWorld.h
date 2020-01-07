#ifndef IKEWORLD_H
#define IKEWORLD_H

#include "math.h"

#include "IkeLib.h"

typedef struct _Vector3{
	double x,y,z;
} Vector3;

typedef struct _Vector2{
	double x,y;
} Vector2;

Vector3* vecNormalize3(Vector3* vector);
Vector2* vecNormalize2(Vector2* vector);

Vector3* vecAdd3(Vector3* a, Vector3* b);
Vector2* vecAdd2(Vector2* a, Vector2* b);

Vector3* vecSub3(Vector3* a, Vector3* b);
Vector2* vecSub2(Vector2* a, Vector2* b);

Vector3* vecNeg3(Vector3* vector);
Vector2* vecNeg2(Vector2* vector);

Vector3* vecMul3(Vector3* vector, double val);
Vector2* vecMul2(Vector2* vector, double val);

Vector3* vecDiv3(Vector3* vector, double val);
Vector2* vecDiv2(Vector2* vector, double val);

typedef struct _Transform3{
	Vector3 position;
	Vector3 velocity;
} Transform3;

typedef struct _Transform2{
	Vector2 position;
	Vector2 velocity;
} Transform2;

struct _GameObject;

typedef struct _GameWorld{
	struct _GameObject** objects;	// object list
	long num_objects;				// number of objects in the world
	long current_obj;				// object ID currently running update()
	int num_rm;						// number of objects removed
} GameWorld;

typedef struct _GameObject{
	void (*render)(Window* window);
	void (*update)(GameWorld* game, double dt);
	long ls_millis;
	void* gamedata_0;
	void* gamedata_1;
	void* gamedata_2;
	void* gamedata_3;
	int gameint_0, gameint_1;
} GameObject;

void handleRigidbody3(Transform3* transform);
void handleRigidbody2(Transform2* transform);


// runs update function of all objects in the GameWorld object.
// returns time taken to do it, in seconds
double simulateWorld(GameWorld* game);

// essentially the GameWorld constructor (except this is c...)
GameWorld* makeGameWorld(GameWorld* output);

// render all objects in a GameWorld object
void renderGameWorld(Window* window, GameWorld* game);

// add a GameObject to a GameWorld
void addGameObject(GameWorld* game, GameObject* object);

// remove a GameObject from a GameWorld via lookup
// note: this function is slow and it's use should be avoided.
//void removeGameObject(GameWorld* game, GameObject* object);

// remove a GameObject from a GameWorld via index
// this is a better function (very fast)
void removeGameObject(GameWorld* game, long index);

// each gameobject has an index in it's game world, this returns the index of the current object
long getCurrentObjectIndex(GameWorld* game);

// looks up a GameObject and returns the index in a gameworld, or -1 if not found
long getObjectIndex(GameWorld* game, GameObject* object);

#endif
