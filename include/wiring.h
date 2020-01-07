#ifndef WIRING_H
#define WIRING_H

#include "blocks.h"

#define WIRE_SHAPE_UD 0
#define WIRE_SHAPE_LR 1
#define WIRE_SHAPE_UL 2
#define WIRE_SHAPE_UR 3
#define WIRE_SHAPE_DL 4
#define WIRE_SHAPE_DR 5

class Wire: public Block {
public:
	Block cached_target;

	uint16_t mat_core;
	uint16_t mat_insul;

	Wire(int shape, uint16_t core, uint16_t insulation);

	// from Block
	uint16_t hardness();
	int64_t forwardMicroOhms(uint8_t side);
	void forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms);

	// from GameObject
	void update(SimWorld* world, uint8_t ms);
	uint32_t microblockVolume();
	uint32_t density();
	uint64_t micrograms();
	uint8_t collidable();
	uint16_t getMaterial();
}

#endif
