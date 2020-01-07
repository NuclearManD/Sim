#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "world.h"
#include "blocks.h"
#include "material.h"
#include "powersupply.h"

extern "C" uint64_t millis();

pthread_t game_thread;
int sig_stop = 0;

int game_rdy = 0;

void*	game_thread_code(void* input){
	// look for game file
	
	/*int fd = open("world.dat", O_RDONLY);
	if (fd < 0){
		printf("world.dat could not be read, generating new world...");
		
	}*/
	SimWorld world;

	game_rdy = 1;
	/*while (!sig_stop){
		world.world_tick(10);
	}*/
	
	
	
	game_rdy = 0;
	return NULL;
}

void	game_begin(){
	pthread_create(&game_thread, NULL, game_thread_code, NULL);
}

void	game_stop(){
	sig_stop = 1;
	pthread_join(game_thread, NULL);
}

int		game_is_ready(){
	return game_rdy;
}
