#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<GL/glew.h>
#include <vector>

#include "ShadowMap.h"

class Light {
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dInensity);

	ShadowMap* GetShadowMap() { return shadowMap; }
	

	~Light();
};

