#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

Mesh::Mesh() : Geometry(MESH) {
	geoColor = glm::vec3(0.6f);
	fileName = "";
}

void Mesh::setFile(string file) {
	fileName = file;
}

Mesh::Mesh(string file) : Geometry(MESH) {
	geoColor = glm::vec3(0.6f);
	fileName = file;
	buildGeometry();
}

void Mesh::refreshColor() {
	for (unsigned int i = 0; i < colors_.size(); i++) {
		colors_[i] = geoColor;
	}
}

glm::vec3 Mesh::getCenter() {
	return center_;
}

static void printVector(glm::vec3 v) {
	cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << endl;
}

void Mesh::buildGeometry() {

	if (fileName.compare("") == 0) {
		return;
	}

	vertices_.clear();
    colors_.clear();
    normals_.clear();
    indices_.clear();

	ifstream myfile(fileName);
	string word;

	int faceCount = 0;
	int vertexCount = 0;

	vector<glm::vec3> tempVertices;

	if (myfile.is_open()) {

		while (myfile >> word) {

			if (word.compare("v") == 0) {
				
				myfile >> word;
				float vx = stof(word);

				myfile >> word;
				float vy = stof(word);

				myfile >> word;
				float vz = stof(word);

				tempVertices.push_back(glm::vec3(vx, vy, vz));
				vertexCount++;

			} else if (word.compare("vn") == 0) {


			} else if (word.compare("f") == 0) {

				myfile >> word;
				string slash = "/";
				string vpos1 = word.substr(0, word.find(slash));
				int i1 = stoi(vpos1);

				myfile >> word;
				string vpos2 = word.substr(0, word.find(slash));
				int i2 = stoi(vpos2);

				myfile >> word;
				string vpos3 = word.substr(0, word.find(slash));
				int i3 = stoi(vpos3);

				glm::vec3 vertex1 = tempVertices[i1 - 1];
				glm::vec3 vertex2 = tempVertices[i2 - 1];
				glm::vec3 vertex3 = tempVertices[i3 - 1];

				glm::vec3 v12 = vertex2 - vertex1;
				glm::vec3 v13 = vertex3 - vertex1;

				glm::vec3 norm = glm::cross(v12, v13);
				glm::vec3 normnorm = glm::normalize(norm);

				normals_.push_back(normnorm);
				normals_.push_back(normnorm);
				normals_.push_back(normnorm);

				vertices_.push_back(vertex1);
				vertices_.push_back(vertex2);
				vertices_.push_back(vertex3);

				faceCount++;
			}
		}

	}

	// indices and colors
    for (unsigned int i = 0; i < vertices_.size(); ++i) {
        colors_.push_back(geoColor);
    }

    for (unsigned int i = 0; i < vertices_.size(); ++i) {
        indices_.push_back(i);
    }

}

Mesh::~Mesh() {}