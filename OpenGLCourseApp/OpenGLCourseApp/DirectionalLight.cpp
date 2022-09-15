#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dInensity, GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dInensity) {
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::useLight(GLfloat ambientIntensityLocation, GLfloat ColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	glUniform3f(ColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

void DirectionalLight::UpdateDirection(float x, float y, float z) {
	direction.x = x;
	direction.y = y;
	direction.z = z;
}

glm::mat4 DirectionalLight::CalculateLightTransform() {
	return lightProj * glm::lookAt(direction * -10.0f, glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}
