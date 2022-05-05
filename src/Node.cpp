#include "Node.h"
#include "scene/Geometry.h"
#include "scene/Sphere.h"

using namespace std;
#include <fstream>
#include <iostream>


static void printVector(glm::vec3 v) {
	cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << endl;
}

Node::Node() {
	parent = NULL;
	vector<Node*> c;
	children = c;
	geometry = NULL;
	hasGeometry = false;
	
	translation = glm::mat4(1.0f);
	rotation = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);

	transformation = glm::mat4(1.0f);

	transVec = glm::vec3(0.0f);
	rotVec = glm::vec3(0.0f);
	scaleVec = glm::vec3(1.0f);

	color = glm::vec3(0.6f, 0.6f, 0.6f);

}
Node::Node(Node* p, vector<Node*> c, Sphere* s) {
	parent = p;
	children = c;
	geometry = s;
	hasGeometry = true;

	translation = glm::mat4(1.0f);
	rotation = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);

	transformation = glm::mat4(1.0f);

	transVec = glm::vec3(0.0f);
	rotVec = glm::vec3(0.0f);
	scaleVec = glm::vec3(1.0f);

	color = glm::vec3(0.6f, 0.6f, 0.6f);

}
void Node::setParent(Node* p) {
	parent = p;
}
void Node::addChild(Node* child) {
	children.push_back(child);
}
void Node::refreshColors() {

	if (!hasGeometry) {
		return;
	}

	geometry->geoColor = color;
	geometry->refreshColor();
}
void Node::setXTrans(float n) {
	transVec[0] = n;
}
void Node::setYTrans(float n) {
	transVec[1] = n;
}
void Node::setZTrans(float n) {
	transVec[2] = n;
}
void Node::setXScale(float n) {
	scaleVec[0] = n;
}
void Node::setYScale(float n) {
	scaleVec[1] = n;
}
void Node::setZScale(float n) {
	scaleVec[2] = n;
}
void Node::setXRotation(float n) {
	rotVec[0] = n;
}
void Node::setYRotation(float n) {
	rotVec[1] = n;
}
void Node::setZRotation(float n) {
	rotVec[2] = n;
}

// Update the transformation matrix
void Node::transform() {
	
	scale = glm::scale(glm::mat4(1.0f), scaleVec);

	translation = glm::translate(glm::mat4(1.0f), transVec);

    //glm::vec3 shapeCenter = origin;

	// Rotation adjustment (center - origin)
    glm::vec3 adjustment  = glm::vec3(0); //-1.0f * shapeCenter;
    glm::vec3 unadjustment = glm::vec3(0); //shapeCenter;

	// Move it to the origin
	glm::mat4 reset = glm::translate(glm::mat4(1.0f), adjustment);
	
	// Move it back
	glm::mat4 rereset = glm::translate(glm::mat4(1.0f), unadjustment);

	glm::mat4 xRotationMatrix = glm::rotate(glm::mat4(1.0f), rotVec.x, glm::vec3(1, 0, 0));
	glm::mat4 yRotationMatrix = glm::rotate(glm::mat4(1.0f), rotVec.y, glm::vec3(0, 1, 0));
	glm::mat4 zRotationMatrix = glm::rotate(glm::mat4(1.0f), rotVec.z, glm::vec3(0, 0, 1));


    rotation = yRotationMatrix * xRotationMatrix; // zRotationMatrix *

    //transformation = translation * reset * rotation * scale * rereset;
    transformation = translation * rotation * scale;

    //noScaleTransformation = translation * reset * rotation * rereset;
    noScaleTransformation = translation * rotation;


	
}
