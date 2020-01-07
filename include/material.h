
#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdint.h>

// 'milli' here is 1/1024 of a whole, not 1/1000

typedef struct s_material {
	const char* name;
	uint16_t hardness;		// 1/16th per one on moh's hardness scale
	int32_t melting_point;	// 1/64th of a degree C, in C (not Kelvin)
	int32_t boiling_point;	// 1/64th of a degree C
	int32_t density;		// grams per block

	// 1024R = pL/(512W) (L and W are in milliblocks)
	// p = 512K + cG/64, where c is 64x temperature in Celsius
	int64_t resistivity_k;	// resistivity base (K)
	int64_t resistivity_g;	// resistivity variance (G)
	
	uint16_t specific_heat; // 1/1024 C gram per joule)
} t_material;

#define MATERIAL_GRANITE	0
#define MATERIAL_IRON		1
#define MATERIAL_DIAMOND	2
#define MATERIAL_GRAPHITE	3
#define MATERIAL_COPPER		4
#define MATERIAL_SIO2		5
#define MATERIAL_WATER		6
#define MATERIAL_SILICON	7
#define MATERIAL_STEEL		8

#define SPEC_HEAT(x) material_list[x].specific_heat
#define NAME_OF(x) material_list[x].name
#define BOILING_POINT(x) material_list[x].boiling_point

extern const t_material material_list[];

int64_t resistivity(int64_t res_k, int64_t res_g, int32_t temp);
int64_t milliohm_resistance(int64_t resistivity, uint32_t milliblock_l, uint32_t milliblock_w);
int64_t microohm_resistance(int64_t resistivity, uint32_t milliblock_l, uint32_t milliblock_w);

#endif
