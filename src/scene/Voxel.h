#ifndef VOXEL_H
#define VOXEL_H

#include <glm/glm.hpp>
#include <stdio.h>
#include <math.h>
#include "../Node.h"

class Voxel {

public:

	Voxel();
	Voxel(float d);

	void setDensity(float);

	float getDensity();

	float density;
	glm::vec4 position;
	Node* node;

	vector<Voxel*> xzNeighbors;
	vector<Voxel*> xyNeighbors;
	vector<Voxel*> yzNeighbors;

	bool push;

	// Store the voxel's position in the buffer
	int x;
	int y;
	int z;

	// Is the voxel part of a 3-connected component?
	bool comp;

	// Is the voxel part of a 2x2 block
	bool block;

	void setColor(glm::vec3);
	void setColor(float, float, float);
	void clearVoxel();

	bool my_equals(Voxel*);


};
#endif
