#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Node.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "Rope.h"
#include "smartpointerhelp.h"

class FileReader {
public:
    Voxel ****voxelBuffer;
    int buffX;
    int buffY;
    int buffZ;
    
	FileReader(string);

	// Returns false if the file does not read successfully
    bool read();

	string fileName;

	// Camera
	float RESOwidth;
	float RESOheight;
	glm::vec3 EYEP;
	glm::vec3 VDIR;
	glm::vec3 UVEC;
	float FOVY;

	// Light
	glm::vec3 LPOS;
	glm::vec3 LCOL;

	// Nodes
    vector<uPtr<Node>> nodes;
    uPtr<Node> rootNode;

	// Rope
    uPtr<Rope> rope;

	
};

#endif
