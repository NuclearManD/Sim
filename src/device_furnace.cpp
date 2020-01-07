
#include "device_furnace.h"
#include "recipe_furnace.h"
#include "material.h"


DeviceFurnace::DeviceFurnace(t_world_coord coord, uint16_t cable, uint16_t rods)
	: Device(coord){
	mat_cable = cable;
	mat_rods = rods;
	
	content = {0, 0, 0, 0, 0, 0};
	react = 0;
	counter = 0;
	internal_temperature = temperature;
}



uint16_t DeviceFurnace::hardness(){
	return material_list[MATERIAL_STEEL].hardness;
}

uint8_t DeviceFurnace::insert(GameObject* obj){
	if (content.milligrams_1 && content.milligrams_2 && content.milligrams_3)
		return 0; // too full.

	uint16_t mat = obj->getMaterial();
	if (mat == 65535)
		return 0; // must accept valid material type

	uint64_t total_mg = content.milligrams_1 + content.milligrams_2 + content.milligrams_3;
	if (mat < content.material_1 || content.milligrams_1 == 0){
		// shift contents over by one.  third will be empty (stays sorted).
		content.material_3 = content.material_2;
		content.milligrams_3 = content.milligrams_2;
		content.material_2 = content.material_1;
		content.milligrams_2 = content.milligrams_1;

		content.material_1 = mat;
		content.milligrams_1 = obj->micrograms() >> 10;

		internal_temperature = (total_mg * internal_temperature) + (content.milligrams_1 * obj->temperature);
		internal_temperature = internal_temperature / (total_mg + content.milligrams_1);

	}else if (mat < content.material_2 || content.milligrams_2 == 0){
		// shift contents over by one.  third will be empty (stays sorted).
		content.material_3 = content.material_2;
		content.milligrams_3 = content.milligrams_2;

		content.material_2 = mat;
		content.milligrams_2 = obj->micrograms() >> 10;

		internal_temperature = (total_mg * internal_temperature) + (content.milligrams_2 * obj->temperature);
		internal_temperature = internal_temperature / (total_mg + content.milligrams_2);

	}else if (mat < content.material_3 || content.milligrams_3 == 0){

		content.material_3 = mat;
		content.milligrams_3 = obj->micrograms() >> 10;

		uint64_t tmp = (total_mg * internal_temperature) + (content.milligrams_3 * obj->temperature);
		internal_temperature = tmp / (total_mg + content.milligrams_3);

	}
	react = 1;
	return 1;
}

uint8_t DeviceFurnace::insert(Liquid* liquid){
	// TODO: implement this
	return 0;
}



uint64_t DeviceFurnace::internalMicroOhms(){
	int64_t res = resistivity(material_list[mat_cable].resistivity_k, material_list[mat_cable].resistivity_g, temperature);
	return microohm_resistance(res, 45, 2);
}

uint8_t DeviceFurnace::efficiency(){
	return 220;
}

uint32_t DeviceFurnace::getSpecificHeat() {
	return SPEC_HEAT(MATERIAL_STEEL);
}


void DeviceFurnace::update(SimWorld* world, uint8_t ms){
	if (react || available_millijoules > 64) {
		react = computeFurnaceOutput(&content, (int64_t*)&available_millijoules, &internal_temperature);
		
		// Check for vapors (they escape)
		while (content.milligrams_1 > 0 && internal_temperature >= BOILING_POINT(content.material_1)){
			content.material_1 = content.material_2;
			content.milligrams_1 = content.milligrams_2;
			content.material_2 = content.material_3;
			content.milligrams_2 = content.milligrams_3;
			
			content.milligrams_3 = 0;
		}
		if (content.milligrams_2 > 0 && internal_temperature >= BOILING_POINT(content.material_2)){
			content.material_2 = content.material_3;
			content.milligrams_2 = content.milligrams_3;
			
			content.milligrams_3 = 0;
		} else if (content.milligrams_3 > 0 && internal_temperature >= BOILING_POINT(content.material_3)){
			content.milligrams_3 = 0;
		}
	}else if ((counter += ms) >= 250){
		counter = 0;
		react = 1;
	}
}

uint32_t DeviceFurnace::microblockVolume(){
	return 3 << 18;
}

uint32_t DeviceFurnace::density(){
	return material_list[MATERIAL_STEEL].density >> 3;
}

uint64_t DeviceFurnace::micrograms(){
	return ((uint64_t)microblockVolume() * density());
}

uint8_t DeviceFurnace::collidable(){
	return 1;
}
