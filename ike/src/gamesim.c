
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IkeLib.h"
#include "IkeWorld.h"

// returns time taken, in seconds.
double simulateWorld(GameWorld* game){
	long ms_start = millis();
	for(game->current_obj=0;game->current_obj<game->num_objects;game->current_obj++){
		GameObject* object = game->objects[game->current_obj];
		if(object!=NULL){
			double dt = millis()-object->ls_millis;
			object->ls_millis = millis();
			
			// divide by 1000 so time is in seconds.
			object->update(game, dt/1000);
		}
	}
	
	// remove removed gameobjects from the list (free memory)
	int cnt = game->num_rm;
	for(int i=0;(cnt>0)&&(i<game->num_objects);i++){
		if(game->objects[i]==NULL){
			game->objects[i] = game->objects[game->num_objects-1];
			game->num_objects--;
			cnt--;
		}
	}
	game->num_rm = 0;
	game->objects = realloc(game->objects, game->num_objects*sizeof(GameObject*));
	return (double)(millis()-ms_start)/1000.0;
}
GameWorld* makeGameWorld(GameWorld* output){
	output->objects = malloc(0);
	output->num_objects = 0;
	return output;
}
void renderGameWorld(Window* window, GameWorld* game){
	
	// here I am copying the GameWorld's object list.
	// This way, when multithreading is used to simulate and render at the same time,
	// removing a gameobject will not mess up rendering.  Avoids race conditions.
	
	long num_obj = game->num_objects;
	
	GameObject** object_list = malloc(num_obj*sizeof(GameObject*));
	memcpy(object_list, game->objects, num_obj*sizeof(GameObject*));
	
	// now we can render safely
	
	for(int i=0;i<num_obj;i++){
		object_list[i]->render(window);
	}
}
void addGameObject(GameWorld* game, GameObject* object){
	game->objects = realloc(game->objects,(game->num_objects+1)*sizeof(GameObject*));
	game->objects[game->num_objects]=object;
	game->num_objects++;
}

// note: this function is slow and it's use should be avoided.
/*void removeGameObject(GameWorld* game, GameObject* object){
 * 
 * 
 * }
 */

// this is a better function (very fast)
void removeGameObject(GameWorld* game, long index){
	if((index!=-1)&&(index<game->num_objects)){
		game->objects[index] = NULL;
		game->num_rm++;
	}else
		printf("Warn: Invalid index %li for IkeLib removeGameObject",index);
}


// each gameobject has an index in it's game world, this returns the index of the current object
long getCurrentObjectIndex(GameWorld* game){
	return game->current_obj;
}

long getObjectIndex(GameWorld* game, GameObject* object){
	for(long i=0;i<game->num_objects;i++){
		if(game->objects[i]==object)return i;
	}
	return -1;
}
