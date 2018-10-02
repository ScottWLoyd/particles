#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>

#include "game.h"
#include "particle_generator.h"

static SpriteRenderer* renderer = NULL;

Game::Game(int width, int height)
	: width(width), height(height)
{
	this->init();
}

void Game::init()
{
	for (uint32_t i = 0; i < ARRAY_COUNT(this->keys); i++)
	{
		this->keys[i] = 0;
	}

	// Load shaders
	load_shader("shaders/2d_vertex.glsl", "shaders/2d_fragment.glsl", NULL, "sprite");
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, -1.0f, 1.0f);
	Shader sprite_shader = get_shader("sprite");
	shader_use(&sprite_shader);
	shader_set_integer(&sprite_shader, "image", 0);
	shader_set_matrix4(&sprite_shader, "projection", projection);

	load_shader("shaders/particle_vertex.glsl", "shaders/particle_fragment.glsl", NULL, "particle");
	Shader part_shader = get_shader("particle");
	shader_use(&part_shader);
	shader_set_integer(&part_shader, "sprite", 0);
	shader_set_matrix4(&part_shader, "projection", projection);	

	load_shader("shaders/instance_part_vert.glsl", "shaders/instance_part_frag.glsl", NULL, "instance");
	Shader inst_shader = get_shader("instance");
	shader_use(&inst_shader);
	shader_set_matrix4(&inst_shader, "projection", projection);

	// Load textures
	load_texture("textures/background.jpg", false, "background");
	load_texture("textures/awesomeface.png", true, "face");
	load_texture("textures/ball.png", true, "ball");
	load_texture("textures/block.png", false, "block");
	load_texture("textures/block_solid.png", false, "block_solid");
	load_texture("textures/paddle.png", true, "paddle");
	load_texture("textures/particle.png", true, "particle");
	load_texture("textures/puff.png", true, "puff");
	load_texture("textures/smoke.png", true, "smoke");
	load_texture("textures/star.png", true, "star");
	load_texture("textures/flame.png", true, "flame");

	renderer = new_sprite_renderer(sprite_shader);

#if 0
	ParticleGenerator* particles = new ParticleGenerator(part_shader, get_texture("flame"), 800, 1);
	particles->size = Vec2(25);
	particles->position = Vec2(this->width / 2, this->height * 2 / 3);
	particles->velocity = Vec2(0);
	particles->speed0 = 150;	particles->speed1 = 300;
	particles->theta0 = 00;	particles->theta1 = 180;
	particles->color0 = Vec4{ 1,0,0,1 }; particles->color1 = Vec4{ 1,1,0.35f,1 };
	particles->gravity = -300;
	particles->drag = Vec2{ 0.995f, 1 };
	this->objects.push_back(particles);
#else
	ParticleGenerator* particles = new ParticleGenerator(inst_shader, get_texture("flame"), 800, 1);
	particles->size = Vec2(25);
	particles->position = Vec2(this->width / 2, this->height * 2 / 3);
	particles->velocity = Vec2(0);
	particles->speed0 = 150;	particles->speed1 = 300;
	particles->theta0 = 00;	particles->theta1 = 180;
	particles->color0 = Vec4{ 1,0,0,1 }; particles->color1 = Vec4{ 1,1,0.35f,1 };
	particles->gravity = -300;
	particles->drag = Vec2{ 0.995f, 1 };
	this->objects.push_back(particles);
#endif
		
	CHECK_ERROR;
}

void Game::process_input(GLfloat dt)
{
	if (this->state == GAME_ACTIVE)
	{
		if (this->keys[GLFW_KEY_D])
		{
		}
		if (this->keys[GLFW_KEY_SPACE])
		{
		}
	}
}

void Game::update(GLfloat dt)
{
	for (GameObject* object : this->objects)
		object->update(dt);
}

void Game::render()
{
	if (this->state == GAME_ACTIVE)
	{
		//Texture2D texture = get_texture("background");
		//draw_sprite(renderer, &texture, Vec2{ 0, 0 }, Vec2{ this->width, this->height });
		
		for (GameObject* object : this->objects)
		{
			object->draw();
		}
	}
}


