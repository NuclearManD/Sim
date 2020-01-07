
#include "material.h"
#include "blocks.h"
#include "world.h"

#include <stdint.h>

#include <stdio.h>



void		Block::raiseTemp(uint32_t celsius64){
	temperature += celsius64;
}

uint8_t		Block::insert(GameObject* obj){
	return 0;
}

uint8_t		Block::insert(Liquid* liquid){
	return 0;
}

uint8_t Block::isBlock(){
	return 1;
}


PureBlock::PureBlock(uint16_t mat, t_world_coord coord){
	material = mat;
	location = coord;
	temperature = 24 << 6; // 24 C
}

uint16_t	PureBlock::hardness(){
	return material_list[material].hardness;
}

int64_t	PureBlock::forwardMicroOhms(uint8_t side){
	int64_t res = resistivity(material_list[material].resistivity_k, material_list[material].resistivity_g, temperature);
	//printf("Block resistance: %f Ohms\n", milliohm_resistance(res, 50, 10) / 1024.f);
	//printf("Block resistance: %f Ohms\n", microohm_resistance(res, 50, 10) / 1048567.f);
	return microohm_resistance(res, 50, 10);
}

void		PureBlock::forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms){
	double milliwatts = (millivolt / 1024.0) * milliamp;
	double joules64 = (milliwatts * ms) / 16000.0;
	//printf("Short: %f V, %f A, %f W, %f J\n", millivolt / 1024.f, milliamp / 1024.f, (float)(milliwatts / 1024.f), (float)(joules64 / 64.f));
	raiseTemp(((int64_t)(joules64 * SPEC_HEAT(material)) / material_list[material].density) >> 10);
}
void		PureBlock::update(SimWorld* world, uint8_t ms){
	if (temperature > material_list[material].melting_point){
		if (world->remove(this)){
			Block* block = new PureLiquidBlock(material, location);
			block->temperature = temperature;
			world->add(block);
			printf("Melted!\n");
			delete this;
		}
	}
}

uint32_t	PureBlock::microblockVolume(){
	return 1<<20;
}

uint32_t	PureBlock::density(){
	return material_list[material].density;
}

uint64_t	PureBlock::micrograms(){
	return ((uint64_t)material_list[material].density) << 20;
}

uint8_t		PureBlock::collidable(){
	return 1;
}

uint16_t	PureBlock::getMaterial(){
	return material;
}

uint64_t powerBlockReturnMicroJoules(Block* block, uint8_t side, uint64_t maxmillivolts, int32_t internal_microohm, uint32_t ms){
	int64_t external_circuit_microohms = block->forwardMicroOhms(side);

	int64_t total_microohm = external_circuit_microohms + internal_microohm;
	printf("external: %f Ohm | total: %f Ohm\n", external_circuit_microohms / 1048576.f, total_microohm / 1048576.f);
	
	// I use doubles here because the numbers get too big for even an int64_t, but after the division they fit again.
	// 10/10 worth the computation cost.
	double kamps = ((double)maxmillivolts) / total_microohm;
	int64_t milliamps = (int64_t)(kamps * 1048576.0);
	int64_t internal_resistor_millivolts = (int64_t)(kamps * internal_microohm);
	int64_t output_millivolts = maxmillivolts - internal_resistor_millivolts;
	
	block->forwardConduct(output_millivolts, milliamps, side, ms);
	
	return internal_resistor_millivolts * milliamps;
}




LiquidBlock::LiquidBlock(t_world_coord coord){
	location = coord;
}


uint16_t LiquidBlock::hardness(){
	return 0;
}

int64_t LiquidBlock::forwardMicroOhms(uint8_t side){
	return 1L << 36;
}


uint32_t LiquidBlock::microblockVolume(){
	
}

uint8_t LiquidBlock::collidable(){
	return 0;
}



PureLiquidBlock::PureLiquidBlock(uint16_t material, t_world_coord coord)
	: LiquidBlock(coord) {
	this->material = material;
}

// from Block
int64_t	PureLiquidBlock::forwardMicroOhms(uint8_t side){
	int64_t res = resistivity(material_list[material].resistivity_k, material_list[material].resistivity_g, temperature) * 2;
	//printf("Block resistance: %f Ohms\n", milliohm_resistance(res, 50, 10) / 1024.f);
	//printf("Block resistance: %f Ohms\n", microohm_resistance(res, 50, 10) / 1048567.f);
	return microohm_resistance(res, 50, 10);
}

void PureLiquidBlock::forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms){
	double milliwatts = (millivolt / 1024.0) * milliamp;
	double joules64 = (milliwatts * ms) / 16000.0;
	raiseTemp(((int64_t)(joules64 * material_list[material].specific_heat) / material_list[material].density) >> 10);
}

// from GameObject
void PureLiquidBlock::update(SimWorld* world, uint8_t ms){
	if (temperature < material_list[material].melting_point){
		if (world->remove(this)){
			Block* block = new PureBlock(material, location);
			block->temperature = temperature;
			delete this;
		}
	}
}

uint32_t PureLiquidBlock::density(){
	return material_list[material].density;
}

uint64_t PureLiquidBlock::micrograms(){
	return ((uint64_t)material_list[material].density) << 20;
}

uint16_t	PureLiquidBlock::getMaterial(){
	return material;
}
