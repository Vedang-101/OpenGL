#pragma once

#include <GL/glew.h>

class Material {
private:
	GLfloat specularIntensity;
	GLfloat shininess;
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();
};

