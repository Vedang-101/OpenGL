#pragma once

#include "Light.h"

class DirectionalLight : public Light {
private:
	glm::vec3 direction;
public:
	DirectionalLight();
	DirectionalLight(	GLfloat shadowWidth, GLfloat shadowHeight,
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dInensity,
						GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void useLight(	GLfloat ambientIntensityLocation, GLfloat ColorLocation, GLfloat diffuseIntensityLocation,
					GLfloat directionLocation);

	glm::mat4 CalculateLightTransform();

	void UpdateDirection(float x, float y, float z);

	~DirectionalLight();
};