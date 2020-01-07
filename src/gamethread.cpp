#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "world.h"
#include "blocks.h"
#include "material.h"
#include "powersupply.h"
#include "device_furnace.h"

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
	
	DeviceFurnace* furnace = new DeviceFurnace({5, 5}, MATERIAL_COPPER, MATERIAL_IRON);
	LiPoBattery* battery = new LiPoBattery({6, 5}, SIDE_LEFT, 4, 10240);
	
	world.add(furnace);
	world.add(battery);
	
	furnace->content = {MATERIAL_GRAPHITE, 300, MATERIAL_SIO2, 400, 0, 0};
	furnace->react = 1; // jumpstart the optimizer to actually react shit
	
	printFurnaceContent(&(furnace->content));
	
	printf("\n\n");
	
	for (int i = 0; i < 35; i++) {
		
		world.world_tick(10);
		world.world_tick(10);
		
		printf("%fC internal\n", furnace->internal_temperature / 64.f);
		printf("Batt: %fC  | Furnace: %fC\n", battery->temperature / 64.f, furnace->temperature / 64.f);
		printFurnaceContent(&(furnace->content));
	}
	
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
