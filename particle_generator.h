#ifndef _PARTICLE_GENERATOR_H_
#define _PARTICLE_GENERATOR_H_

#include <vector>

#include <GL/glew.h>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

struct Particle {
	Vec2 position;
	Vec2 velocity;
	float scale;
	Vec4 color;
	float life;
};

struct ParticleGenerator : GameObject {
public:
	std::vector<Particle> particles;
	// Particle parameters
	float rate; // Particles per second
	float life; // Seconds
	float speed0, speed1;	// Pixels per second
	float theta0, theta1;	// Degrees
	Vec4 color0, color1;	
	float gravity;	// Pixels per second^2
	Vec2 drag;	// Percentage

	ParticleGenerator(Shader shader, Texture2D texture, float rate, float life = 1.0f);
	~ParticleGenerator();

	void update(float dt);
	void draw();

private:
#if 0
	GLuint vao;
#else
	GLuint billboard_vbo;
	GLuint pos_vbo;
	GLuint color_vbo;
#endif
	Shader shader;
	Texture2D texture;
	uint32_t last_used_particle = 0;
	uint32_t amount;

	std::vector<Vec4> particle_position_size_data;
	std::vector<Vec4> particle_color_data;

	void respawn_particle(Particle* particle);
	uint32_t first_unused_particle();
};

#endif
