#include "SpotLight.h"

SpotLight::SpotLight() : PointLight() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
	isOn = true;
}

SpotLight::SpotLight(	GLfloat shadowWidth, GLfloat shadowHeight,
						GLfloat near, GLfloat far,
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dInensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat xDir, GLfloat yDir, GLfloat zDir,
						GLfloat con, GLfloat lin, GLfloat exp,
						GLfloat edg) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dInensity, xPos, yPos, zPos, con, lin, exp) {
	
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = edg;
	procEdge = cosf(glm::radians(edge));
	isOn = true;
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ColorLocation, GLuint diffuseIntensityLocation,
	GLuint PositionLocation, GLuint DirectionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint procEdgeLocation) {

	glUniform3f(ColorLocation, color.x, color.y, color.z);

	if (isOn) {
		glUniform1f(ambientIntensityLocation, ambientIntensity);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}
	else {
		glUniform1f(ambientIntensityLocation, 0);
		glUniform1f(diffuseIntensityLocation, 0);
	}
	glUniform3f(PositionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(DirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(procEdgeLocation, procEdge);
}

void SpotLight::UpdateDirection(float x, float y, float z) {
	direction.x = x;
	direction.y = y;
	direction.z = z;
}

void SpotLight::UpdateDirection(glm::vec3 Dir) {
	direction.x = Dir.x;
	direction.y = Dir.y;
	direction.z = Dir.z;
}

SpotLight::~SpotLight() {
}
