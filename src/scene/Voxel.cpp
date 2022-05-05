#include "Voxel.h"

Voxel::Voxel() {
	density = 0;
	push = false;
	comp = false;
	block = false;
}

Voxel::Voxel(float d) {	
	density = d;
}
void Voxel::setDensity(float d) {
	density = d;
}
float Voxel::getDensity() {
	return density;
}

void Voxel::setColor(glm::vec3 c) {
	if (density == 0) {
		return;
	}

	node->color = c;
	node->refreshColors();

}

void Voxel::setColor(float r, float g, float b) {
	this->setColor(glm::vec3(r, g, b));
}

void Voxel::clearVoxel() {
	density = 0;
	node->hasGeometry = false;
}

bool Voxel::my_equals(Voxel* other) {

	if (x != other->x) {
		return false;
	}

	if (y != other->y) {
		return false;
	}

	if (z != other->z) {
		return false;
	}

	return true;

}