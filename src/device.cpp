
#include "device.h"

Device::Device(t_world_coord coord){
	location = coord;
	available_millijoules = 0;
}

int64_t Device::forwardMicroOhms(uint8_t side){
	int64_t accumulator = (16 * available_millijoules);
	return accumulator * accumulator + internalMicroOhms();
}

void Device::forwardConduct(uint64_t millivolt, int64_t milliamp, uint8_t side, uint32_t ms){
	double milliwatts = (millivolt / 1024.0) * milliamp;
	double millijoules = (milliwatts * ms) / 1000.0;
	uint8_t efficiency = this->efficiency();
	
	available_millijoules += (((int64_t)millijoules) * efficiency) >> 8;
	
	efficiency = 256 - efficiency;
	raiseTemp((((int64_t)(millijoules * getSpecificHeat()) / (micrograms() >> 10)) * efficiency) >> 8);
}

