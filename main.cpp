#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "rendering/game.hpp"
#include "rendering/camera.hpp"
#include "core/shader.hpp"
#include "core/entity.hpp"
#include "core/model.hpp"
#include "rendering/window.hpp"
#include "rendering/texture2D.hpp"

#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"
#include "maths/mat4.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "audio/audio_manager.hpp"

//#define STB_IMAGE_IMPLEMENTATION
#include "core/stb_image.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void callbacks();

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
const std::string& title = "OpenGL";

Entity entity;
Window window(SCR_WIDTH, SCR_HEIGHT);
AudioManager audio_test;

using namespace math;

int main()
{
	window.Init(SCR_WIDTH, SCR_HEIGHT, "HATRED ENGINE");

	callbacks();
	Shader shader("main_vertex.glsl", "main_frag.glsl");
	Shader lampShader("lamp_vertex.glsl", "lamp_fragment.glsl");
	Model testModel("assimp_test/untitled.obj");
	//Model testModel("entity_test/nanosuit.obj");


	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	
	unsigned int floorTexture = Texture2D::loadTexture("SpectrumEx.jpg");
	//unsigned int floorTexture = Texture2D::loadTexture("floor.png");
	shader.use();
	shader.SetInteger("texture1", 0);
	
	audio_test.play2D("breakout.mp3", GL_TRUE);
	
	while (!glfwWindowShouldClose(window.window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		shader.use();
		shader.SetVector3f("light.direction", -0.2f, -1.0f, -0.3f);
		shader.SetVector3f("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));

		// light properties
		shader.SetVector3f("light.ambient", 0.2f, 0.2f, 0.2f);
		shader.SetVector3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		shader.SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		shader.SetFloat("material.shininess", 32.0f);
		
		// Input
		//Engine.ProcessInput(deltaTime);
		entity.Input(window.window, deltaTime);
		// Update
		//Engine.Update(deltaTime);
		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		entity.Draw(shader, testModel);

		//Engine.Render();
		
		glfwSwapBuffers(window.window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
}

void callbacks()
{
	glfwSetFramebufferSizeCallback(window.window, entity.framebuffer_size_callback);
	glfwSetKeyCallback(window.window, key_callback);
	glfwSetCursorPosCallback(window.window, entity.mouse_callback);
	glfwSetScrollCallback(window.window, entity.scroll_callback);
}