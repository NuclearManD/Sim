
#include "powersupply.h"

#include <stdio.h>

Battery::Battery(int64_t start_charge, uint8_t facing){
	printf("Initial charge: %f J\n", start_charge / 1024.f);
	charge_millijoules = start_charge;
	direction = facing;
	timer = 0;
}

void Battery::update(SimWorld* world, uint8_t ms){
	if (charge_millijoules <= 100)
		return;
	if (timer == 0){
		// This code makes the rest of the function run only every other tick.
		timer = ms;
		return;
	}else
		ms += timer;
	t_world_coord target = location;
	NEXT_TO(target, direction);
	Block* block = world->getBlock(target, 0);
	if (!block){
		timer = 0; // skip tick
		return;
	}
	int64_t external_circuit_microohms = block->forwardMicroOhms(OPPOSITE(direction));
	
	int64_t maxmillivolts = millivolts();
	
	int64_t total_microohm = external_circuit_microohms + discharge_microohms;
	double kamps = ((double)maxmillivolts) / total_microohm;
	double charge_change = ((maxmillivolts * kamps) * (1024 * (uint16_t)ms)) / 1000;
	if (charge_change < 10){
		timer = 0; // skip a tick
		return; // no energy output
	}
	
	if (charge_change > charge_millijoules){
		maxmillivolts = (maxmillivolts / charge_change) * charge_millijoules;
		kamps = ((double)maxmillivolts) / total_microohm;
		charge_change = ((maxmillivolts * kamps * 1024) * ms) / 1000;
	}
	charge_millijoules -= (int64_t)charge_change;
	
	int64_t milliamps = (int64_t)(kamps * 1048576.0);
	int64_t internal_resistor_millivolts = (int64_t)(kamps * discharge_microohms);
	int64_t output_millivolts = maxmillivolts - internal_resistor_millivolts;
	
	block->forwardConduct(output_millivolts, milliamps, OPPOSITE(direction), ms);
	
	double heat_joules = internal_resistor_millivolts * kamps * ms / 1000.0;
	int64_t C1024 = (int64_t) ((specific_heat_Cg_per_KJ() * heat_joules) / micrograms());
	raiseTemp((int32_t) (C1024 / 16));
	//printf("%f J wasted as heat\n", heat_joules);
	//printf("Discharge: %f V, %f A, %f C, %f J\n", output_millivolts / 1024.f, milliamps / 1024.f, temperature / 64.f, charge_change / 1024.f);
	timer = 0;
}


uint8_t Battery::insert(GameObject* obj){
	return 0;
}

uint8_t Battery::insert(Liquid* liquid){
	return 0;
}

int64_t Battery::forwardMicroOhms(uint8_t side){
	if (charge_millijoules >= max_millijoules)
		return 137438953472; // 128 KOhm
	return (charge_microohms * (1048576 + (charge_millijoules << 21) / max_millijoules)) >> 20;
}

void Battery::forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms){
	int64_t microjoules = (millivolt * milliamp * ms) / 1000;
	double heat_joules;
	if (charge_millijoules < max_millijoules){
		//printf("%f J charged into battery\n", ((microjoules >> 10) * charge_efficiency_of_256) / 262144.f);
		charge_millijoules += ((microjoules >> 10) * charge_efficiency_of_256) >> 8;
		heat_joules = (microjoules * (256 - charge_efficiency_of_256)) / 268435456.0;
	}else{
		//printf("OVERCHARGED: %f of %f J\n", charge_millijoules / 1024.f, max_millijoules / 1024.f);
		heat_joules = microjoules / 1048576;
	}
	//printf("%f J wasted as heat\n", heat_joules);
	int64_t C1024 = (int64_t) (((specific_heat_Cg_per_KJ() << 20) * heat_joules) / micrograms());
	raiseTemp((int32_t) (C1024 >> 4));
}

LiPoBattery::LiPoBattery(t_world_coord coord, uint8_t facing, int num_cells, uint64_t internal_microohms)
	: Battery(num_cells * 24062336L, facing){
	location = coord;
	charge_microohms = discharge_microohms = internal_microohms;
	charge_efficiency_of_256 = 218;
	max_millijoules = num_cells * 34062336L;
	cells = num_cells;
}

uint64_t LiPoBattery::millivolts(){
	double vratio = (1024.0 * charge_millijoules) / max_millijoules;
	if (vratio > 1050) // 105% (CAUTION!)
		return 4405L * cells;
	else if (vratio > 921) // 90%
		return ((vratio * 4301L) / 1024) * cells;
	else if (153 < vratio)
		return ((vratio + 3410L) / 2) * cells;
	else
		return ((vratio * 23244L) / 1024) * cells;
}

uint64_t LiPoBattery::specific_heat_Cg_per_KJ(){
	return 1024; // yes it's almost exactly one.
	// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.1011.4415&rep=rep1&type=pdf
}

uint16_t LiPoBattery::hardness(){
	return 17;
}

uint32_t LiPoBattery::microblockVolume(){
	return (cells * 295) >> 16;
}

uint32_t LiPoBattery::density(){
	return 2619834;
}

uint64_t LiPoBattery::micrograms(){
	return 48234496UL*cells;
}
uint8_t LiPoBattery::collidable(){
	return 0;
}
