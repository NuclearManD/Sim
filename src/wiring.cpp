
#include "wiring.h"


Wire::Wire(int shape, uint16_t core, uint16_t insulation);

// from Block
uint16_t Wire::hardness();
int64_t Wire::forwardMicroOhms(uint8_t side);
void Wire::forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms);

// from GameObject
void Wire::update(SimWorld* world, uint8_t ms);
uint32_t Wire::microblockVolume();
uint32_t Wire::density();
uint64_t Wire::micrograms();
uint8_t Wire::collidable();
uint16_t Wire::getMaterial();
