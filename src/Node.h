#ifndef NODE_H
#define NODE_H
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "scene/Geometry.h"
#include "scene/Sphere.h"
#include "scene/Cylinder.h"
#include "scene/Cube.h"
#include "scene/Mesh.h"
#include <string>

using namespace std;

class Node {

public: 

	Node();
	Node(Node*, vector<Node*>, Sphere*);
	Node(Node*, vector<Node*>, Cylinder*);
	Node(Node*, vector<Node*>, Cube*);

	bool hasGeometry;

	void setParent(Node*);
	void addChild(Node*);
	void transform();
	void refreshColors();

	void removeChild(Node*);

	void setXTrans(float);
	void setYTrans(float);
	void setZTrans(float);

	void setXScale(float);
	void setYScale(float);
	void setZScale(float);

	void setXRotation(float);
	void setYRotation(float);
	void setZRotation(float);

	glm::vec3 transVec;
	glm::vec3 rotVec;
	glm::vec3 scaleVec;

	Node* parent;
	vector<Node*> children;
	Geometry* geometry;

	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scale;
	glm::mat4 transformation;
	glm::mat4 totalTransformation;

	glm::vec3 origin;

	string name;
	glm::vec3 color;

	glm::mat4 genTranslation(glm::vec3);
	glm::mat4 noScaleTransformation;

};
#endif
