#ifndef CUBE_H
#define CUBE_H

#include "Geometry.h"

class Cube : public Geometry
{
public:
    Cube();
    virtual ~Cube();

    virtual void buildGeometry();
	virtual void refreshColor();

	virtual glm::vec3 getCenter();

private:
    glm::vec3 center_;
	float xlength_;
	float ylength_;
	float zlength_;
};

#endif
