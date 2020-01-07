
#ifndef BLOCKS_H
#define BLOCKS_H

#include "material.h"
#include "world.h"


#define SIDE_LEFT 0 // x minus
#define SIDE_RIGHT 1 // x plus
#define SIDE_DOWN 2 // y minus
#define SIDE_UP 3 // y plus

#define NEXT_TO(a, b) if ((b) < 2)(a).x+=2*(b)-1;else (a).y+=2*(b)-5;
#define OPPOSITE(x) (x)^1


class Block: public GameObject {
public:
	virtual uint16_t hardness() = 0;

	// Objects and melted things, returns 1 on success, 0 on failure
	virtual uint8_t insert(GameObject* obj);

	// Liquids, 1 = success 0 = failure
	virtual uint8_t insert(Liquid* liquid);

	// Electricity
	virtual int64_t forwardMicroOhms(uint8_t side) = 0;
	virtual void forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms) = 0;

	// Heat
	virtual void raiseTemp(uint32_t celsius64);
	
	uint8_t isBlock();
};

class PureBlock: public Block {
public:
	uint16_t material;
	
	uint64_t timer;

	PureBlock(uint16_t material, t_world_coord coord);

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
};

class LiquidBlock: public Block {
public:
	LiquidBlock(t_world_coord coord);

	// from Block
	uint16_t hardness();
	virtual int64_t forwardMicroOhms(uint8_t side);

	// from GameObject
	uint32_t microblockVolume();
	uint8_t collidable();
};

class PureLiquidBlock: public LiquidBlock {
public:
	uint16_t material;
	
	uint64_t timer;

	PureLiquidBlock(uint16_t material, t_world_coord coord);

	// from Block
	int64_t forwardMicroOhms(uint8_t side);
	void forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms);

	// from GameObject
	void update(SimWorld* world, uint8_t ms);
	uint32_t density();
	uint64_t micrograms();
	uint16_t getMaterial();
};

uint64_t powerBlockReturnMicroJoules(Block* block, uint8_t side, uint64_t maxmillivolts, int32_t internal_microohm, uint32_t ms);

#endif
