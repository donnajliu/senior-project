#ifndef SPHERE_H
#define SPHERE_H

#include "Geometry.h"

class Sphere : public Geometry {

public:
    Sphere();
    virtual ~Sphere();

    virtual void buildGeometry();
	virtual void refreshColor();

	virtual glm::vec3 getCenter();

private:
    glm::vec3 center_;
    float radius_;
};

#endif