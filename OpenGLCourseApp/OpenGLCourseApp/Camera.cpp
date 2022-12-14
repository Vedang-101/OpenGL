#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::Update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::KeyControl(bool *keys, GLfloat deltaTime) {
	GLfloat velocity = deltaTime * moveSpeed;

	if (keys[GLFW_KEY_W])
		position += front * velocity;
	if (keys[GLFW_KEY_A])
		position -= right * velocity;
	if (keys[GLFW_KEY_S])
		position -= front * velocity;
	if (keys[GLFW_KEY_D])
		position += right * velocity;
	if(keys[GLFW_KEY_SPACE])
		position += up * velocity;
	if (keys[GLFW_KEY_LEFT_CONTROL])
		position -= up * velocity;
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	Update();
}

glm::vec3 Camera::GetCameraPosition() {
	return position;
}

glm::vec3 Camera::GetCameraDirection() {
	return glm::normalize(front);
}

Camera::~Camera()
{
}
