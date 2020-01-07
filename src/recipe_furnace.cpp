
#include "recipe_furnace.h"

#include "material.h"

#include <stdint.h>
#include <stdio.h>

void printFurnaceContent(t_furnace_content* content) {
	uint64_t total_milligrams = (content->milligrams_1) + (content->milligrams_2) + (content->milligrams_3);
	printf("Furnace:\n");
	if (content->milligrams_1)
		printf("\t1: %lumg %s\n", content->milligrams_1, NAME_OF(content->material_1));
	if (content->milligrams_2)
		printf("\t2: %lumg %s\n", content->milligrams_2, NAME_OF(content->material_2));
	if (content->milligrams_3)
		printf("\t3: %lumg %s\n", content->milligrams_3, NAME_OF(content->material_3));
	printf("\tTotal %lumg\n", total_milligrams);
}

// expects input to be sorted.  content will be sorted on return if it is sorted on call.
uint8_t computeFurnaceOutput(t_furnace_content* content, int64_t* millijoules, int32_t *temperature) {
	uint16_t mat1, mat2, mat3;
	uint64_t mg1, mg2, mg3;
	uint64_t total_milligrams;
	int64_t C_ug;
	uint64_t a, b, c;
	uint64_t tmp;
	uint8_t out = 0;
	
	total_milligrams = (mg1 = content->milligrams_1) + (mg2 = content->milligrams_2) + (mg3 = content->milligrams_3);
	
	mat1 = content->material_1;
	mat2 = content->material_2;
	mat3 = content->material_3;
	
	if (total_milligrams != 0) {
		
		// Compute the recipes.
		// Recipe computation uses starting temperature.  If no recipies execute, then the temperature increases and the next tick we try again.
		
		if (mat1 == MATERIAL_GRAPHITE || mat1 == MATERIAL_DIAMOND) {
			// carbon reduction, or steelmaking, or something like that.
			
			if (mat2 == MATERIAL_SIO2 && *temperature >= (1573 << 6)) {
				// 2C + SiO2 -> Si + 2CO
				// 24 + 60.1 = 28.1 + 56
				// Carbon Monoxide is destroyed (as if released into atmosphere)

				a = (mg1 << 4) / 24;
				b = (mg2 << 4) / 60;
				if (a > 16 && b > 16) {
					mg1 = mg1 << 4;
					mg2 = mg2 << 4;
					if (a > b) {
						// Carbon left over, material2 is now the produced silicon.
						mg2 = b * 28;
						mg1 -= b * 24;
						mat2 = MATERIAL_SILICON;
						a = b; // a should be the reaction size
					} else if (a < b) {
						// SiO2 left over, need to exchange material1 and material2,
						// with material2 as the produced silicon, to keep them sorted.
						mg1 = mg2 - a * 60;
						mg2 = a * 28;
						mat2 = MATERIAL_SILICON;
					} else {
						// none left over.  material2 is now none and material1 is the
						// produced silicon.
						mg2 = 0;
						mg1 = a * 28;
						mat1 = MATERIAL_SILICON;
					}
					mg1 = mg1 >> 4;
					mg2 = mg2 >> 4;
					*millijoules -= 16 * a; // this reaction costs energy
					out = 1;
				}
			}else if (mat2 == MATERIAL_IRON && *temperature >= 1559 << 6) {
				// 1% Carbon, 99% Iron
				// Carbon Monoxide is destroyed (as if released into atmosphere)

				a = mg1 << 4;
				b = (mg2 << 4) / 99;
				if (a > 16 && b > 16) {
					mg1 = mg1 << 4;
					mg2 = mg2 << 4;
					if (a > b) {
						// Carbon left over, material2 is now the produced steel.
						mg2 = b * 100;
						mg1 -= b * 1;
						mat2 = MATERIAL_STEEL;
					} else if (a < b) {
						// Iron left over, need to exchange material1 and material2,
						// with material2 as the produced steel, to keep them sorted.
						mg1 = mg2 - a * 1;
						mg2 = a * 100;
						mat2 = MATERIAL_STEEL;
					} else {
						// none left over.  material2 is now none and material1 is the
						// produced steel.
						mg2 = 0;
						mg1 = a * 100;
						mat1 = MATERIAL_STEEL;
					}
					mg1 = mg1 >> 4;
					mg2 = mg2 >> 4;
					out = 1;
				}
			}
			
			// if materials change caused an unsort, resort.
			// mat3 was unchanged, and mat1 < mat2, so there are some
			// cases we needn't check for.
			if (mat2 > mat3 && mg3 != 0){
				tmp = mat3;
				mat3 = mat2;
				mat2 = tmp;
				tmp = mg3;
				mg3 = mg2;
				mg2 = tmp;
				if (mat1 > mat2) {
					tmp = mat1;
					mat1 = mat2;
					mat2 = tmp;
					tmp = mg1;
					mg1 = mg2;
					mg2 = tmp;
				}
			}
			
		}
		
		content->milligrams_1 = mg1;
		content->milligrams_2 = mg2;
		content->milligrams_3 = mg3;
		content->material_1 = mat1;
		content->material_2 = mat2;
		content->material_3 = mat3;
		
	}
	
	// combined specific heat, roughly microgram accuracy.  Uses Rule of Mixtures. microgram celsius per J
	if (mg1){
		C_ug =  (mg1 << 16) / SPEC_HEAT(mat1);
		if (mg2){
			C_ug += (mg2 << 16) / SPEC_HEAT(mat2);
			if (mg3)
				C_ug += (mg3 << 16) / SPEC_HEAT(mat3);
		}
	}

	int32_t increment;

	if (C_ug) {
		C_ug = ((total_milligrams = mg1 + mg2 + mg3) << 26) / C_ug;
		//printf("%li mJ, C_ug = %li\n", *millijoules, C_ug);
		// multiply joules for celsius micrograms as units
		C_ug = (C_ug * (*millijoules)) >> 10;
		//printf("%li C * mJ\n", C_ug);
		// increase temperature accordingly
		increment = (int32_t)((C_ug / (int64_t)total_milligrams) >> 4);
	} else {
		increment = (*millijoules) << 4;
	}
	
	if (increment != 0){
		*temperature += increment;
		*millijoules = 0;
	} else
		out = 1;
	
	return out;
}
