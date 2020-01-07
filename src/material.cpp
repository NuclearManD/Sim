
#include "material.h"

#include <stdio.h>

// 1024R = pL/(512W) (L and W are in milliblocks)
// p = 512K + cG/64, where c is 64x temperature in Celsius

int64_t resistivity(int64_t res_k, int64_t res_g, int32_t temp){
	return (res_k << 9) + ((temp * res_g) >> 6);
}

int64_t milliohm_resistance(int64_t resistivity, uint32_t milliblock_l, uint32_t milliblock_w){
	return ((resistivity * (int64_t)milliblock_l) / (int64_t)milliblock_w) >> 9;
}

int64_t microohm_resistance(int64_t resistivity, uint32_t milliblock_l, uint32_t milliblock_w){
	return (((resistivity << 1) * (int64_t)milliblock_l) / (int64_t)milliblock_w);
}

const t_material material_list[] = {
	[MATERIAL_GRANITE] = {
		.name = "Granite",
		.hardness = 104,
		.melting_point = 1243 << 6,
		.boiling_point = 2436 << 6,
		.density = 2750000,
		.resistivity_k = 5242880000,
		.resistivity_g = 26214400,
		.specific_heat = 1296,
	},
	[MATERIAL_IRON] = {
		.name = "Iron",
		.hardness = 64,
		.melting_point = 1538 << 6,
		.boiling_point = 2861 << 6,
		.density = 7874000,
		.resistivity_k = 2,
		.resistivity_g = 1,
		.specific_heat = 2156,
	},
	[MATERIAL_DIAMOND] = {
		.name = "Diamond",
		.hardness = 160,
		.melting_point = 3685 << 6,
		.boiling_point = 3684 << 6,
		.density = 3510000,
		
		// melts before hyperconductance
		.resistivity_k = 2L << 43,
		.resistivity_g = -1L << 39,
		.specific_heat = 1969,
	},
	[MATERIAL_GRAPHITE] = {
		.name = "Graphite",
		.hardness = 24,
		.melting_point = 3685 << 6,
		.boiling_point = 3684 << 6,
		.density = 2150000,
		
		// Note: hyperconductance at 3448C
		.resistivity_k = 330,
		.resistivity_g = -49,
		.specific_heat = 1447,
	},
	[MATERIAL_COPPER] = {
		.name = "Copper",
		.hardness = 56,
		.melting_point = 1083 << 6,
		.boiling_point = 2567 << 6,
		.density = 8960000,
		
		.resistivity_k = 1,
		.resistivity_g = 1,
		.specific_heat = 2626,
	},
	[MATERIAL_SIO2] = {
		.name = "SiO2",
		.hardness = 110,
		.melting_point = 1600 << 6,
		.boiling_point = 2230 << 6,
		.density = 2358333,
		
		.resistivity_k = 2UL << 37,
		.resistivity_g = 2UL << 34,
		.specific_heat = 1356,
	},
	[MATERIAL_WATER] = {
		.name = "Water",
		.hardness = 72,
		.melting_point = 0 << 6,
		.boiling_point = 100 << 6,
		.density = 997000,
		
		.resistivity_k = 8942881000,
		.resistivity_g = 13814400,
		.specific_heat = 245,
	},
	[MATERIAL_SILICON] = {
		.name = "Silicon",
		.hardness = 104,
		.melting_point = 1410 << 6,
		.boiling_point = 2355 << 6,
		.density = 2320000,
		
		.resistivity_k = 31509709,
		.resistivity_g = -15728640,
		.specific_heat = 1365,
	},
	[MATERIAL_STEEL] = {
		.name = "Steel",
		.hardness = 6 << 4,
		.melting_point = 1512 << 6,
		.boiling_point = 2862 << 6,
		.density = 8252293,
		
		.resistivity_k = 3,
		.resistivity_g = 2,
		.specific_heat = 2154,
	},
};
