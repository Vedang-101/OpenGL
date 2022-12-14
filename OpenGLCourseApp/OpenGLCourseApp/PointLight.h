#pragma once
#include "Light.h"
#include "OmniShadowMap.h"

class PointLight: public Light {
protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;

	GLfloat farPlane;
public:
	PointLight();
	PointLight(	GLfloat shadowWidth, GLfloat shadowHeight,
				GLfloat near, GLfloat far,
				GLfloat red, GLfloat green, GLfloat blue,
				GLfloat aIntensity, GLfloat dInensity,
				GLfloat xPos, GLfloat yPos, GLfloat zPos,
				GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ColorLocation, GLuint diffuseIntensityLocation,
		GLuint PositionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	GLfloat GetFarPlane() { return farPlane; }

	glm::vec3 GetPosition() { return position; }

	void UpdatePosition(float x, float y, float z);
	void UpdatePosition(glm::vec3 Pos);

	~PointLight();
};

