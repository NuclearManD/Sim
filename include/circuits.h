
#ifndef CIRCUITS_H
#define CIRCUITS_H

#include <stdint.h>

#define FULL_ON_V 2.92
#define FULL_OFF_uR 1024UL*1024UL*1024UL*1024UL*1024UL
#define CAP_DIV_PER_MS 2

class CircuitPowerSwitch {
public:
	
	// Falstad:
	
	/*
$ 1 0.000005 10.20027730826997 50 5 43
f 512 176 560 176 32 1.5 1
w 560 224 560 192 0
r 416 176 464 176 0 1000
c 464 176 464 224 0 0.000009999999999999999 4.997411078210868
w 512 224 560 224 0
d 464 144 416 144 2 default
w 416 144 416 176 0
w 464 144 464 176 0
w 464 224 512 224 0
w 464 176 512 176 0
d 512 224 512 176 2 default
g 512 224 512 240 0
w 416 176 400 176 0
w 560 160 576 160 0
w 560 224 576 224 0
w 464 224 400 224 0
	*/

	uint32_t inp_pF_;
	uint32_t inp_mR_;
	uint32_t max_mV_;
	uint32_t on_mR_;
	
	uint16_t cap_chrg_of_64k_;
	

	CircuitPowerSwitch(uint32_t inp_pF, uint32_t inp_mR, uint32_t max_mV, uint32_t on_uR);

	uint64_t conductionMicroOhms();
	uint64_t conductPrimary(uint64_t millivolt, int64_t milliamp, uint32_t ms);
	uint64_t applyControl(uint64_t millivolt, int64_t ext_mR, uint32_t ms); // returns microamps

	// from GameObject
	void update(SimWorld* world, uint8_t ms);
	uint64_t micrograms();
};

#endif
