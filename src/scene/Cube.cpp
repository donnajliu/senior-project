#include "Cube.h"

static const float PI = 3.141592653589f;

// Creates a unit cube with its front corner at (0, 0, 0)
Cube::Cube() :
    Geometry(CUBE),
    center_(glm::vec3(0.0f, 0.0f, 0.0f)),
	xlength_(1.0f),
	ylength_(1.0f),
	zlength_(1.0f)
{
    geoColor = glm::vec3(0.6f);
	buildGeometry();
}

Cube::~Cube() {}

void Cube::refreshColor() {
	for (unsigned int i = 0; i < colors_.size(); i++) {
		colors_[i] = geoColor;
	}
}

glm::vec3 Cube::getCenter() {
	return center_;
}

void Cube::buildGeometry() {
    vertices_.clear();
    colors_.clear();
    normals_.clear();
    indices_.clear();

	glm::vec3 p0(0.5f, 0.5f, 0.5f);
	glm::vec3 p1(-0.5f, 0.5f, 0.5f);
	glm::vec3 p2(-0.5f, -0.5f, 0.5f);
	glm::vec3 p3(0.5f, -0.5f, 0.5f);
	
	glm::vec3 p4(0.5f, -0.5f, -0.5f);
	glm::vec3 p5(0.5f, 0.5f, -0.5f);
	glm::vec3 p6(-0.5f, 0.5f, -0.5f);
	glm::vec3 p7(-0.5f, -0.5f, -0.5f);

	glm::vec3 upNormal(0, 1, 0);
	glm::vec3 downNormal(0, -1, 0);

	glm::vec3 leftNormal(-1, 0, 0);
	glm::vec3 rightNormal(1, 0, 0);

	glm::vec3 frontNormal(0, 0, 1);
	glm::vec3 backNormal(0, 0, -1);

	// Front face (+z)
	vertices_.push_back(p0);
	vertices_.push_back(p1);
	vertices_.push_back(p2);

	normals_.push_back(frontNormal);
	normals_.push_back(frontNormal);
	normals_.push_back(frontNormal);

	vertices_.push_back(p0);
	vertices_.push_back(p2);
	vertices_.push_back(p3);

	normals_.push_back(frontNormal);
	normals_.push_back(frontNormal);
	normals_.push_back(frontNormal);

	// Back face (-z)
	vertices_.push_back(p4);
	vertices_.push_back(p5);
	vertices_.push_back(p6);

	normals_.push_back(backNormal);
	normals_.push_back(backNormal);
	normals_.push_back(backNormal);

	vertices_.push_back(p4);
	vertices_.push_back(p6);
	vertices_.push_back(p7);

	normals_.push_back(backNormal);
	normals_.push_back(backNormal);
	normals_.push_back(backNormal);

	// Right face (+x)
	vertices_.push_back(p0);
	vertices_.push_back(p3);
	vertices_.push_back(p4);

	normals_.push_back(rightNormal);
	normals_.push_back(rightNormal);
	normals_.push_back(rightNormal);

	vertices_.push_back(p0);
	vertices_.push_back(p4);
	vertices_.push_back(p5);

	normals_.push_back(rightNormal);
	normals_.push_back(rightNormal);
	normals_.push_back(rightNormal);

	// Left face (-x)
	vertices_.push_back(p2);
	vertices_.push_back(p1);
	vertices_.push_back(p6);

	normals_.push_back(leftNormal);
	normals_.push_back(leftNormal);
	normals_.push_back(leftNormal);

	vertices_.push_back(p2);
	vertices_.push_back(p6);
	vertices_.push_back(p7);

	normals_.push_back(leftNormal);
	normals_.push_back(leftNormal);
	normals_.push_back(leftNormal);
	
	// Top face (+y)
	vertices_.push_back(p0);
	vertices_.push_back(p5);
	vertices_.push_back(p6);

	normals_.push_back(upNormal);
	normals_.push_back(upNormal);
	normals_.push_back(upNormal);

	vertices_.push_back(p0);
	vertices_.push_back(p6);
	vertices_.push_back(p1);

	normals_.push_back(upNormal);
	normals_.push_back(upNormal);
	normals_.push_back(upNormal);

	// Bottom face (-y)
	vertices_.push_back(p3);
	vertices_.push_back(p2);
	vertices_.push_back(p7);

	normals_.push_back(downNormal);
	normals_.push_back(downNormal);
	normals_.push_back(downNormal);

	vertices_.push_back(p3);
	vertices_.push_back(p7);
	vertices_.push_back(p4);

	normals_.push_back(downNormal);
	normals_.push_back(downNormal);
	normals_.push_back(downNormal);

    // indices and colors
    for (unsigned int i = 0; i < vertices_.size(); ++i) {
        colors_.push_back(geoColor);
    }

    for (unsigned int i = 0; i < vertices_.size(); ++i) {
        indices_.push_back(i);
    }
}
