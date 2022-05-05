#ifndef CAMERA_H_

#define CAMERA_H_

#include <glm/glm.hpp>
#include <stdio.h>
#include <math.h>

class Camera {
public:

	Camera();
    Camera(unsigned int w, unsigned int h);
    Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp);
    Camera(const Camera &c);

	glm::vec3 getC();
	glm::vec3 getU();
	glm::vec3 getE();

	void setC(glm::vec3);
	void setU(glm::vec3);
	void setE(glm::vec3);

	float getTheta();
	float getPhi();

	void setTheta(float);
	void setPhi(float);

	glm::vec3 getA();
	glm::vec3 getB();
	glm::vec3 getM();
	glm::vec3 getH();
	glm::vec3 getV();

	void setA();
	void setB();
	void setM();
	void setH();
	void setV();

    // NEW CAMERA
    float fovy;
    unsigned int width, height;  // Screen dimensions
    float near_clip;  // Near clip plane distance
    float far_clip;  // Far clip plane distance

    //Computed attributes
    float aspect;

    glm::vec3 eye,      //The position of the camera in world space
              ref,      //The point in world space towards which the camera is pointing
              look,     //The normalized vector from eye to ref. Is also known as the camera's "forward" vector.
              up,       //The normalized vector pointing upwards IN CAMERA SPACE. This vector is perpendicular to LOOK and RIGHT.
              right,    //The normalized vector pointing rightwards IN CAMERA SPACE. It is perpendicular to UP and LOOK.
              world_up; //The normalized vector pointing upwards IN WORLD SPACE. This is primarily used for computing the camera's initial UP vector.
              //V,        //Represents the vertical component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.
              //H;        //Represents the horizontal component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.

    glm::mat4 getViewProj();
    glm::mat4 getView();
    glm::mat4 getProj();

    void RecomputeAttributes();

    void RotateAboutUp(float deg);
    void RotateAboutRight(float deg);

    void RotateTheta(float deg);
    void RotatePhi(float deg);

    void TranslateAlongLook(float amt);
    void TranslateAlongRight(float amt);
    void TranslateAlongUp(float amt);

    void Zoom(float amt);

    void Reset();

private:
	float theta;	// Half-angle
	float phi;		// Half-angle

	glm::vec3 C;	// Viewing vector direction
	glm::vec3 U;	// Up vector
	glm::vec3 E;	// Eye world coordinates

	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 M;
	glm::vec3 H;
	glm::vec3 V;
};

#endif
