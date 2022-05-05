#ifndef CYLINDER_H
#define CYLINDER_H

#include "Geometry.h"

class Cylinder : public Geometry
{
public:
    Cylinder();
    virtual ~Cylinder();

    virtual void buildGeometry();
	virtual void refreshColor();

	virtual glm::vec3 getCenter();

private:
    glm::vec3 center_;
    float radius_;
    float height_;
};

#endif