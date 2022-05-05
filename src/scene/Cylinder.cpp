#include "Cylinder.h"

static const float PI = 3.141592653589f;

// Creates a unit cylinder centered at (0, 0, 0)
Cylinder::Cylinder() :
    Geometry(CYLINDER),
    center_(glm::vec3(0.f, 0.f, 0.f)),
    radius_(0.5f),
    height_(1.0f)
{
    geoColor = glm::vec3(0.6f);
	buildGeometry();
}

Cylinder::~Cylinder() {}

void Cylinder::refreshColor() {
	for (unsigned int i = 0; i < colors_.size(); i++) {
		colors_[i] = geoColor;
	}
}

glm::vec3 Cylinder::getCenter() {
	return center_;
}

void Cylinder::buildGeometry()
{
    vertices_.clear();
    colors_.clear();
    normals_.clear();
    indices_.clear();

    unsigned short subdiv = 20;
    float dtheta = 2 * PI / subdiv;

    glm::vec4 point_top(0.0f, 0.5f * height_, radius_, 1.0f),
        point_bottom (0.0f, -0.5f * height_, radius_, 1.0f);

    vector<glm::vec3> cap_top, cap_bottom;

    // top and bottom cap vertices
    for (int i = 0; i < subdiv + 1; ++i) {
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), i * dtheta, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), center_);

        cap_top.push_back(glm::vec3(translate * rotate * point_top));
        cap_bottom.push_back(glm::vec3(translate * rotate * point_bottom));
    }

    //Create top cap.
    for ( int i = 0; i < subdiv - 2; i++) {
        vertices_.push_back(cap_top[0]);
        vertices_.push_back(cap_top[i + 1]);
        vertices_.push_back(cap_top[i + 2]);
    }
    //Create bottom cap.
    for (int i = 0; i < subdiv - 2; i++) {
        vertices_.push_back(cap_bottom[0]);
        vertices_.push_back(cap_bottom[i + 1]);
        vertices_.push_back(cap_bottom[i + 2]);
    }
    //Create barrel
    for (int i = 0; i < subdiv; i++) {
        //Right-side up triangle
        vertices_.push_back(cap_top[i]);
        vertices_.push_back(cap_bottom[i + 1]);
        vertices_.push_back(cap_bottom[i]);
        //Upside-down triangle
        vertices_.push_back(cap_top[i]);
        vertices_.push_back(cap_top[i + 1]);
        vertices_.push_back(cap_bottom[i + 1]);
    }

    // create normals
    glm::vec3 top_centerpoint(0.0f , 0.5f * height_ , 0.0f),
        bottom_centerpoint(0.0f, -0.5f * height_, 0.0f);
    glm::vec3 normal(0, 1, 0);

    // Create top cap.
    for (int i = 0; i < subdiv - 2; i++) {
        normals_.push_back(normal);
        normals_.push_back(normal);
        normals_.push_back(normal);
    }
    // Create bottom cap.
    for (int i = 0; i < subdiv - 2; i++) {
        normals_.push_back(-normal);
        normals_.push_back(-normal);
        normals_.push_back(-normal);
    }

    // Create barrel
    for (int i = 0; i < subdiv; i++) {
        //Right-side up triangle
        normals_.push_back(glm::normalize(cap_top[i] - top_centerpoint));
        normals_.push_back(glm::normalize(cap_bottom[i + 1] - bottom_centerpoint));
        normals_.push_back(glm::normalize(cap_bottom[i] - bottom_centerpoint));
        //Upside-down triangle
        normals_.push_back(glm::normalize(cap_top[i] - top_centerpoint));
        normals_.push_back(glm::normalize(cap_top[i + 1] - top_centerpoint));
        normals_.push_back(glm::normalize(cap_bottom[i + 1] - bottom_centerpoint));
    }

    // indices and colors
    glm::vec3 color (0.6f, 0.6f, 0.6f);
    for (unsigned int i = 0; i < vertices_.size(); ++i) {
        colors_.push_back(geoColor);
    }

    for (unsigned int i = 0; i < vertices_.size(); ++i) {
        indices_.push_back(i);
    }
}
