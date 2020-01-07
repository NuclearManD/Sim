#include "gamethread.h"

#include <unistd.h>
#include <stdio.h>

#include "recipe_furnace.h"
#include "material.h"

int main(int argc, char** argv){
	char buffer[1];
	game_begin();
	//read(0, buffer, 1);
	game_stop();
	
	return 0;
}
