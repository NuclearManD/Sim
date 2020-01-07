#ifndef WORLD_H
#define WORLD_H

#include "material.h"
#include <stdint.h>

typedef struct s_world_coord {
	uint32_t x, y;
	uint16_t o1;
	uint16_t r1;
	uint16_t o2;
	uint16_t r2;
	uint16_t o3;
	uint16_t r3;
} t_world_coord;

class Liquid {
public:
	int32_t temperature;
	void update(uint8_t ms);
	virtual uint64_t micrograms() = 0;
	virtual uint32_t getMeltingPoint() = 0;
	virtual uint32_t getBoilingPoint() = 0;
};

class PureLiquid: public Liquid {
public:
	uint16_t material;
	uint64_t n_micrograms;

	PureLiquid(uint16_t material, uint64_t ug);

	uint64_t micrograms();
	uint32_t getMeltingPoint();
	uint32_t getBoilingPoint();
	uint32_t getDensity();
};

class SimWorld;
class Block;

class GameObject {
public:
	t_world_coord location;
	int32_t temperature;
	virtual void update(SimWorld* world, uint8_t ms) = 0;

	// Properties
	virtual uint32_t microblockVolume() = 0;
	virtual uint32_t density() = 0;
	virtual uint64_t micrograms() = 0;
	
	virtual uint8_t collidable() = 0;
	
	virtual uint8_t isBlock();
	virtual uint16_t getMaterial();
};

// 64x64 chunk on the planet surface
class _PlanetChunk{
public:
	GameObject** objects;
	int num_objects;
	int arr_size;
	
	_PlanetChunk();
	
	void add(GameObject* object);
	uint8_t remove(GameObject* object);
	void update(SimWorld* world, uint8_t ms);

	GameObject* get(int x, int y, int index = 0);
	Block* getBlock(int x, int y, int index = 0);
};

#define PLANET_TILES 64
#define PLANET_SIZE PLANET_TILES*PLANET_TILES*64*64

class SimWorld {
public:
	_PlanetChunk* chunks[PLANET_TILES][PLANET_TILES];
	_PlanetChunk* populated[PLANET_TILES * PLANET_TILES];
	int num_chunks_populated;
	uint64_t num_millis;
	
	SimWorld();
	
	void world_tick(uint8_t ms = 10);
	void add(GameObject* object);
	uint8_t remove(GameObject* object);
	
	GameObject* get(t_world_coord where, int index = 0);
	Block* getBlock(t_world_coord where, int index = 0);
	uint8_t move(GameObject* obj, t_world_coord to);
};

#endif
