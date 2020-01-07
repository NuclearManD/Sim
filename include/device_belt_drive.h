
#ifndef DEVICE_BELT_DRIVE_H
#define DEVICE_BELT_DRIVE_H

#include "world.h"
#include "device.h"
#include "recipe_furnace.h"
#include "circuits.h"

#include <stdint.h>

class DeviceBeltDrive: public Device {
public:

	CircuitPowerSwitch* power_switch;

	DeviceBeltDrive(t_world_coord coord, uint16_t cable, uint16_t rods);

	// from Block
	uint16_t hardness();
	uint8_t insert(GameObject* obj);
	uint8_t insert(Liquid* liquid);

	// from Device
	uint64_t internalMicroOhms();
	uint8_t efficiency();
	uint32_t getSpecificHeat();

	// from GameObject
	void update(SimWorld* world, uint8_t ms);
	uint32_t microblockVolume();
	uint32_t density();
	uint64_t micrograms();
	uint8_t collidable();
};

#endif
