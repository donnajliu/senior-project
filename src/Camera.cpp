#include "Camera.h"

#include <la.h>
#include <iostream>


static const float PI = 3.141592653589f;

// Empty constructor
Camera::Camera() {

	theta = 0;
	phi = 0;

	C = glm::vec3(0.0f);
	U = glm::vec3(0.0f);
	E = glm::vec3(0.0f);

	A = glm::vec3(0.0f);
	B = glm::vec3(0.0f);
	M = glm::vec3(0.0f);
	H = glm::vec3(0.0f);
	V = glm::vec3(0.0f);

}

// Calculating A, B, H, M, and V
void Camera::setA() {
	A = glm::cross(C, U);
}
void Camera::setB() {
	B = glm::cross(A, C);
}
void Camera::setM() {
	M = E + C;
}
void Camera::setH() {
	float lenC = glm::length(C);
	float lenA = glm::length(A);
	glm::vec3 num = A * lenC;
	float radTheta = theta * (PI / 180);
	num = num * tan(radTheta);
	H = num / lenA;
	
}
void Camera::setV() {
	float lenC = glm::length(C);
	float lenB = glm::length(B);
	glm::vec3 num = B * lenC;
	float radPhi = phi * (PI / 180);
	num = num * tan(radPhi);
	V = num / lenB;
}

// Getting A, B, H, M, and V
glm::vec3 Camera::getA() {
	return A;
}
glm::vec3 Camera::getB() {
	return B;
}
glm::vec3 Camera::getM() {
	return M;
}
glm::vec3 Camera::getH() {
	return H;
}
glm::vec3 Camera::getV() {
	return V;
}

// Getting all the values
glm::vec3 Camera::getC() {
	return C;
}
glm::vec3 Camera::getU() {
	return U;
}
glm::vec3 Camera::getE() {
	return E;
}
float Camera::getTheta() {
	return theta;
}
float Camera::getPhi() {
	return phi;
}

// Setting all the values
void Camera::setC(glm::vec3 newC) {
	C = newC;
}
void Camera::setU(glm::vec3 newU) {
	U = newU;
}
void Camera::setE(glm::vec3 newE) {
	E = newE;
}
void Camera::setTheta(float newTheta) {
	theta = newTheta;
}
void Camera::setPhi(float newPhi) {
	phi = newPhi;
}

//// OTHER CAMERA

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = glm::tan(fovy/2);
    float len = glm::length(ref - eye);
    aspect = width/height;
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(fovy, width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::getProj()
{
    return glm::perspective(fovy, width / (float)height, near_clip, far_clip);
}

void Camera::Reset()
{
    fovy = 45.f;
    eye = glm::vec3(0, 0, 12);
    ref = glm::vec3(0, 0, 0);
    world_up = glm::vec3(0, 1, 0);
    RecomputeAttributes();
}

void Camera::RotateAboutUp(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}

void Camera::RotateTheta(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
    eye = eye - ref;
    eye = glm::vec3(rotation * glm::vec4(eye, 1.f));
    eye = eye + ref;
    RecomputeAttributes();
}

void Camera::RotatePhi(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
    eye = eye - ref;
    eye = glm::vec3(rotation * glm::vec4(eye, 1.f));
    eye = eye + ref;
    RecomputeAttributes();
}

void Camera::Zoom(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
}

void Camera::TranslateAlongLook(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
    ref += translation;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}
