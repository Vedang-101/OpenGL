#pragma once
#include "PointLight.h"
class SpotLight : public PointLight {
private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
	bool isOn;
public:
	SpotLight();
	SpotLight(	GLfloat shadowWidth, GLfloat shadowHeight,
				GLfloat near, GLfloat far,
				GLfloat red, GLfloat green, GLfloat blue,
				GLfloat aIntensity, GLfloat dInensity,
				GLfloat xPos, GLfloat yPos, GLfloat zPos, 
				GLfloat xDir, GLfloat yDir, GLfloat zDir,
				GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg);

	void UseLight(GLuint ambientIntensityLocation, GLuint ColorLocation, GLuint diffuseIntensityLocation,
		GLuint PositionLocation, GLuint DirectionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint procEdgeLocation);

	void UpdateDirection(float x, float y, float z);
	void UpdateDirection(glm::vec3 Dir);

	void Toggle() { isOn = !isOn; }

	~SpotLight();
};

