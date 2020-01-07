
#ifndef DEVICE_H
#define DEVICE_H

#include "blocks.h"

#include <stdint.h>

class Device: public Block {
public:
	uint64_t available_millijoules;

	Device(t_world_coord coord);
	virtual uint64_t internalMicroOhms() = 0;
	virtual uint8_t efficiency() = 0;
	virtual uint32_t getSpecificHeat() = 0;

	// from Block
	int64_t forwardMicroOhms(uint8_t side);
	void forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms);
};

#endif
