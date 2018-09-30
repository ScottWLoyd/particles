#define _CRT_SECURE_NO_WARNINGS

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "bo_math.h"
#include "util.h"
#include "game.h"
#include "shader.h"
#include "texture.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "particle_generator.h"
#include "util.cpp"
#include "game.cpp"
#include "shader.cpp"
#include "texture.cpp"
#include "resource_manager.cpp"
#include "sprite_renderer.cpp"
#include "game_object.cpp"
#include "particle_generator.cpp"

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
Game* bustout;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			bustout->keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE)
		{
			bustout->keys[key] = GL_FALSE;
		}
	}
}

int main(int argc, char** argv)
{
	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bustout", NULL, NULL);
	glfwMakeContextCurrent(window);

	char* version = (char*)glGetString(GL_VERSION);
	printf("Version: %s\n", version);


	glewExperimental = GL_TRUE;
	glewInit();
	CHECK_ERROR;

	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bustout = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

	GLfloat delta_time = 0.0f;
	GLfloat last_frame = 0.0f;

	bustout->state = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat current_frame = (GLfloat)glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		glfwPollEvents();
		
		// process input
		bustout->process_input(delta_time);

		// update
		bustout->update(delta_time);

		// render
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		bustout->render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}