#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// An abstract base class for geometry in the scene graph.
class Geometry
{
public:
    // Enums for the types of geometry that your scene graph is required to contain.
    // Feel free to add more.
    enum geometryType {CUBE, SPHERE, CYLINDER, MESH, LINE};
    explicit Geometry(geometryType);
    virtual ~Geometry();

	glm::vec3 geoColor;
    virtual void refreshColor() = 0; // Can be impl

    // Function for building vertex data, i.e. vertices, colors, normals, indices.
    // Implemented in Sphere and Cylinder.
    virtual void buildGeometry() = 0;

    virtual glm::vec3 getCenter() = 0; // Can be impl

    // Getters
    const vector<glm::vec3>& getVertices() const
    {
        return vertices_;
    };
    const vector<glm::vec3>& getNormals() const
    {
        return normals_;
    };
    const vector<glm::vec3>& getColors() const
    {
        return colors_;
    };
    const vector<unsigned int>& getIndices() const
    {
        return indices_;
    };

    unsigned int getVertexCount() const
    {
        return vertices_.size();
    };
    unsigned int getIndexCount() const
    {
        return indices_.size();
    };

    const geometryType getGeometryType() const
    {
        return type_;
    };


protected:
    geometryType type_;

    vector<glm::vec3> vertices_;        // vertex buffer
    vector<glm::vec3> normals_;         // normal buffer
    vector<glm::vec3> colors_;          // color buffer
    vector<unsigned int> indices_;      // index buffer

};

#endif
