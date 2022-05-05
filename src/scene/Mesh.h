#ifndef MESH_H
#define MESH_H

#include "Geometry.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

class Mesh : public Geometry {

public:
	Mesh();
	Mesh(string);
	virtual ~Mesh();

	void setFile(string);

	virtual void buildGeometry();
	virtual void refreshColor();

	virtual glm::vec3 getCenter();

	string fileName;

private:
    glm::vec3 center_;

};

#endif
