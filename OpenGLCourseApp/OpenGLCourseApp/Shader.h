#pragma once

#include "CommonValues.h"

#include <stdio.h>
#include<string>
#include<iostream>
#include<fstream>

#include<GL/glew.h>
#include<glm/gtc/type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader {
private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition;
	GLuint uniformSpecularIntensity, uniformShininess, uniformTexture;
	GLuint uniformDirectionalLightTransform, uniformDirectionalShadowMap;
	GLuint uniformOmniLightPos, uniformOmniFarPlane, uniformLightMatrices[6];

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLights[MAX_SPOT_LIGHTS];

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() { return uniformProjection; }
	GLuint GetViewLocation() { return uniformView; }
	GLuint GetModelLocation() { return uniformModel; }
	GLuint GetUniformEyePositionLocation() { return uniformEyePosition; }

	GLuint GetUniformSpecularIntensityLocation() { return uniformSpecularIntensity; }
	GLuint GetUniformShininessLocation() { return uniformShininess; }
	
	GLuint GetOmniLightPosLocation() { return uniformOmniLightPos; }
	GLuint GetOmniFarPlaneLocation() { return uniformOmniFarPlane; }

	void SetDirectionalLight(DirectionalLight *dLight);
	void SetPointLights(PointLight *pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight *sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetLightMatrices(std::vector<glm::mat4> lighMatrices);

	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

	void UseShader();
	void ClearShader();

	~Shader();
};

