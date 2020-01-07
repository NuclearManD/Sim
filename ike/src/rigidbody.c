#include "IkeWorld.h"

/*
 * rigidbody.c : code for handling rigidbodies and simulating physics
*/

// very simple right now, collision handling should be added later.

void handleRigidbody3(Transform3* transform){
	vecAdd3(&(transform->position),&(transform->velocity));
}
void handleRigidbody2(Transform2* transform){
	vecAdd2(&(transform->position),&(transform->velocity));
}
