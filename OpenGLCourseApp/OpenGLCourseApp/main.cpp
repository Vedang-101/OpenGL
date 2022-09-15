#define STB_IMAGE_IMPLEMENTATION

#include<stdio.h>
#include<string.h>
#include<cmath>
#include<vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Material.h"

#include "Model.h"

#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
const int WIDTH = 1080, HEIGHT = 608;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0;
GLuint uniformSpecularIntensity = 0, uniformShininess = 0;
GLuint uniformOmniLightPos = 0, uniformOmniFarPlane = 0;

unsigned int pointLightCount = 0, spotLightCount = 0;

Window mainWindow;
Mesh Plane;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

Material shinyMaterial;
Material dullMaterial;

Model xWing;
Model blackHawk;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CalcAvgNormals(unsigned int* indices, unsigned int indicesCount, GLfloat *vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indicesCount; i += 3) {
		unsigned int in0 = indices[i + 0] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1 + 0] - vertices[in0 + 0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2 + 0] - vertices[in0 + 0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0 + 0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;
		
		vertices[in1 + 0] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2 + 0] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset + 0], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset + 0] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
};

void CreateObjects() {
	GLfloat vertices[] = {
	//		x,	   y,     z,    u,    v,   nx,   ny,   nz
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f, 10.0f,  0.0f, 0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,  0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	CalcAvgNormals(indices, 12, vertices, 32, 8, 5);
/*
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
*/
	Plane = Mesh();
	Plane.CreateMesh(floorVertices, floorIndices, 32, 6);
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directionalShadowMap.vert", "Shaders/directionalShadowMap.frag");

	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omniShadowMap.vert", "Shaders/omniShadowMap.frag", "Shaders/omniShadowMap.geom");
}

void RenderScene() {
	glm::mat4 model;

	//Plane Model
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Plane.RenderMesh();

	//ShadowMap
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-15.0f, -2.0f, -15.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mainLight.GetShadowMap()->Read(GL_TEXTURE1);
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Plane.RenderMesh();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	xWing.RenderModel();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackHawk.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light) {
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight *light) {
	omniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformOmniFarPlane = omniShadowShader.GetOmniFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformOmniFarPlane, light->GetFarPlane());
	
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	
	
	glViewport(0, 0, WIDTH, HEIGHT);

	// Clear Window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();
	
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetUniformEyePositionLocation();

	uniformShininess = shaderList[0].GetUniformShininessLocation();
	uniformSpecularIntensity = shaderList[0].GetUniformSpecularIntensityLocation();

	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	shaderList[0].Validate();

	RenderScene();
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	xWing = Model();
	xWing.LoadModel("Models/Matilda.obj");

	blackHawk = Model();
	blackHawk.LoadModel("Models/uh60.obj");

	mainLight = DirectionalLight(	2048.0f, 2048.0f,
									1.0f, 0.53f, 0.3f,
									0.1f, 0.9f,
								  -10.0f, -12.0f, 18.5f);

	pointLights[0] = PointLight( 512.0f, 512.0f,
								 0.01f, 100.0f,
								 0.0f, 0.0f, 1.0f,
								 0.0f, 1.0f,
								 3.0f, 0.0f, 1.0f,
								 0.3f, 0.1f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight( 512.0f, 512.0f,
								 0.01f, 100.0f,
								 0.0f, 1.0f, 0.0f,
								 0.0f, 1.0f,
								 0.0f, 1.0f, 1.0f,
								 0.3f, 0.1f, 0.1f);
	pointLightCount++;

	spotLights[0] = SpotLight(	512.0f, 512.0f,
								0.01f, 100.0f,
								1.0f, 1.0f, 0.0f,
								0.0f, 1.0f,
								0.0f, 2.0f, 5.0f,
								0.0f, -1.0f, 0.0f,
								0.3f, 0.1f, 0.1f,
								20.0f);
	spotLightCount++;
	
	spotLights[1] = SpotLight(	512.0f, 512.0f,
								0.01f, 100.0f,
								1.0f, 1.0f, 1.0f,
								0.0f, 3.0f,
								0.0f,  0.0f, 0.0f,
							   -3.0f, -1.0f, 0.0f,
								0.3f, 0.1f, 0.1f,
								20.0f);
	spotLightCount++;
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	GLfloat theta = 0;

	// Loop untill window closed
	while (!mainWindow.GetShouldClose()) {
		GLfloat now = glfwGetTime(); //returns time per frame in seconds
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();
		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXMouseChange(), mainWindow.GetYMouseChange());
		mainLight.UpdateDirection(sin(theta * toRadians), -0.5f, cos(theta * toRadians));
		
		theta += 20 * deltaTime;
		if (theta > 360)
			theta = 0.0f;

		if (mainWindow.GetKeys()[GLFW_KEY_L]) {
			spotLights[0].Toggle();
			mainWindow.GetKeys()[GLFW_KEY_L] = false;
		}

		glm::vec3 lowerLight = camera.GetCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].UpdatePosition(lowerLight);
		spotLights[0].UpdateDirection(camera.GetCameraDirection());

		DirectionalShadowMapPass(&mainLight);
		
		for (size_t i = 0; i < pointLightCount; i++)
			OmniShadowMapPass(&pointLights[i]);
		for (size_t i = 0; i < spotLightCount; i++)
			OmniShadowMapPass(&spotLights[i]);
		
		RenderPass(projection, camera.CalculateViewMatrix());

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}