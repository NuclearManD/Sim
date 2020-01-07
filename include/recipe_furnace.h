
#ifndef RECIPE_FURNACE_H
#define RECIPE_FURNACE_H

#include <stdint.h>

typedef struct s_furnace_content {
	uint16_t material_1;
	uint64_t milligrams_1;
	
	uint16_t material_2;
	uint64_t milligrams_2;
	
	uint16_t material_3;
	uint64_t milligrams_3;
} t_furnace_content;

// expects input to be sorted.  content will be sorted on return if it is sorted on call.
uint8_t computeFurnaceOutput(t_furnace_content* content, int64_t* millijoules, int32_t *temperature);

void printFurnaceContent(t_furnace_content* content);

#endif
