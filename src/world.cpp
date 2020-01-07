
#include "world.h"
#include "blocks.h"

#include <stdlib.h>
#include <stdio.h>

void Liquid::update(uint8_t ms){
}

PureLiquid::PureLiquid(uint16_t mat, uint64_t ug){
	n_micrograms = ug;
	material = mat;
}

uint64_t PureLiquid::micrograms(){
	return n_micrograms;
}

uint32_t PureLiquid::getMeltingPoint(){
	return material_list[material].melting_point;
}

uint32_t PureLiquid::getBoilingPoint(){
	return material_list[material].boiling_point;
}

uint32_t PureLiquid::getDensity(){
	return material_list[material].density;
}

uint8_t GameObject::isBlock(){
	return 0;
}

uint16_t GameObject::getMaterial(){
	return 65535;
}

_PlanetChunk::_PlanetChunk(){
	num_objects = 0;
	arr_size = 4;
	objects = (GameObject**)malloc(sizeof(GameObject*)*arr_size);
}

void _PlanetChunk::add(GameObject* object){
	if (num_objects == arr_size) {
		objects = (GameObject**)realloc(objects, sizeof(GameObject*)*(arr_size + 4));
		arr_size += 4;
	}
	objects[num_objects] = object;
	num_objects++;
}

uint8_t _PlanetChunk::remove(GameObject* object) {
	for (int i = 0; i < num_objects; i++) {
		if(objects[i] == object) {
			objects[i] = objects[num_objects - 1];
			num_objects--;
			if (arr_size - num_objects > 8){
				objects = (GameObject**)realloc(objects, sizeof(GameObject*)*(arr_size - 4));
				arr_size -= 4;
			}
			return 1;
		}
	}
	return 0;
}

void _PlanetChunk::update(SimWorld* world, uint8_t ms) {
	for (int i = 0; i < num_objects; i++)
		objects[i]->update(world, ms);
}

GameObject* _PlanetChunk::get(int x, int y, int index){
	int cnt = 0;
	for (int i = 0; i < num_objects; i++) {
		if (objects[i]->location.x == x && objects[i]->location.y == y){
			cnt++;
			if (cnt > index)
				return objects[i];
		}
	}
	return nullptr;
}

Block* _PlanetChunk::getBlock(int x, int y, int index){
	int cnt = 0;
	for (int i = 0; i < num_objects; i++) {
		GameObject* obj = objects[i];
		if (obj->isBlock()){
			t_world_coord loc = obj->location;
			if (loc.x == x && loc.y == y){
				cnt++;
				if (cnt > index)
					return (Block*)obj;
			}
		}
	}
	return nullptr;
}

SimWorld::SimWorld(){
	for (int x = 0; x < PLANET_TILES; x++)
		for (int y = 0; y < PLANET_TILES; y++)
			chunks[x][y] = nullptr;
	num_chunks_populated = 0;
}

void SimWorld::world_tick(uint8_t ms){
	for (int i = 0; i < num_chunks_populated; i++)
		populated[i]->update(this, ms);
	num_millis += ms;
}

void SimWorld::add(GameObject* object){
	if (!object)
		return;
	t_world_coord coord = object->location;
	_PlanetChunk* chunk = chunks[coord.x >> 6][coord.y >> 6];
	if (!chunk){
		chunk = chunks[coord.x >> 6][coord.y >> 6] = new _PlanetChunk();
		populated[num_chunks_populated] = chunk;
		num_chunks_populated++;
	}
	chunk->add(object);
}

uint8_t SimWorld::remove(GameObject* object){
	if (!object)
		return 0;
	t_world_coord coord = object->location;
	_PlanetChunk* chunk = chunks[coord.x >> 6][coord.y >> 6];
	if (!chunk)
		return 0;
	if (chunk->remove(object)){
		//if (chunk->num_objects == 0)
		//	delete chunk;
		// TODO: remove from list of populated chunks here if it has become empty
		return 1;
	}
	return 0;
}

GameObject* SimWorld::get(t_world_coord where, int index){
	_PlanetChunk* chunk = chunks[where.x >> 6][where.y >> 6];
	if (!chunk)
		return nullptr;
	return chunk->get(where.x, where.y, index);
}

Block* SimWorld::getBlock(t_world_coord where, int index){
	_PlanetChunk* chunk = chunks[where.x >> 6][where.y >> 6];
	if (!chunk)
		return nullptr;
	return chunk->getBlock(where.x, where.y, index);
}

uint8_t SimWorld::move(GameObject* obj, t_world_coord to){
	if (!obj)
		return 0;
	if (!remove(obj))
		return 0;
	obj->location = to;
	add(obj);
	return 1;
}
