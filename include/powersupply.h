
#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

#include "blocks.h"

class Battery: public Block {
public:
	uint64_t max_millijoules;
	uint64_t discharge_microohms;
	uint64_t charge_microohms;
	int64_t charge_millijoules;
	uint8_t direction;
	uint8_t charge_efficiency_of_256;
	uint16_t timer;
	Battery(int64_t start_charge, uint8_t facing);
	virtual uint64_t millivolts() = 0;
	virtual uint64_t specific_heat_Cg_per_KJ() = 0;
	
	void update(SimWorld* world, uint8_t ms);
	
	uint8_t insert(GameObject* obj);
	uint8_t insert(Liquid* liquid);
	int64_t forwardMicroOhms(uint8_t side);
	void forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms);
};

class LiPoBattery: public Battery {
public:
	int cells;

	LiPoBattery(t_world_coord coord, uint8_t facing, int num_cells, uint64_t internal_microohms = 51200);
	uint64_t millivolts();
	uint64_t specific_heat_Cg_per_KJ();
	
	uint16_t hardness();
	uint32_t microblockVolume();
	uint32_t density();
	uint64_t micrograms();
	uint8_t collidable();
};

#endif
