#include "particle_generator.h"

uint32_t ParticleGenerator::first_unused_particle()
{
	for (uint32_t i = this->last_used_particle; i < this->amount; i++)
	{
		if (this->particles[i].life <= 0)
		{
			this->last_used_particle = i;
			return i;
		}
	}
	for (uint32_t i = 0; i < this->last_used_particle; i++)
	{
		if (this->particles[i].life <= 0)
		{
			this->last_used_particle = i;
			return i;
		}
	}
	this->last_used_particle = 0;
	return 0;
}

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, float rate, float life)
	: shader(shader), texture(texture), rate(rate), life(life)
{	
	this->speed0 = this->speed1 = 0;
	this->theta0 = this->theta1 = 0;
	this->color0 = this->color1 = Vec4{ 1 };
	this->gravity = 0;
	this->drag = Vec2{ 1.0f };

	GLuint vbo;
	float particle_quad[] = {
		0, 1, 0, 1,
		1, 0, 1, 0,
		0, 0, 0, 0,

		0, 1, 0, 1,
		1, 1, 1, 1,
		1, 0, 1, 0,
	};

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(this->vao);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	
	this->amount = this->rate * this->life;
	for (uint32_t i = 0; i < this->amount; i++)
	{
		Particle part;
		this->particles.push_back(part);
	}
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::respawn_particle(Particle* particle)
{
	Vec2 offset = Vec2{ this->size.x * ((rand() % 100) - 50) / 100.0f,
						this->size.y * ((rand() % 100) - 50) / 100.0f };
	float color = 0.5f + ((rand() % 100) / 100.0f);
	float speed = LERP(this->speed0, (rand() % 100) / 100.0f, this->speed1);
	float theta = LERP(this->theta0, (rand() % 100) / 100.0f, this->theta1);
	particle->position = this->position + offset;
	particle->color = lerp(this->color0, this->color1, (rand() % 100) / 100.0f);
	particle->scale = Vec2(10);
	particle->life = this->life;
	float total_angle = RADIANS(this->rotation + theta);
	particle->velocity = speed * Vec2{ cos(total_angle), -sin(total_angle) };
}

void ParticleGenerator::update(float dt)
{
	uint32_t new_particles = (uint32_t)(dt * this->rate);
	for (uint32_t i = 0; i < new_particles; i++)
	{
		int unused_particle = this->first_unused_particle();
		this->respawn_particle(&this->particles[unused_particle]);
	}
	for (uint32_t i = 0; i < this->amount; i++)
	{
		Particle* p = &this->particles[i];
		if (p->life > 0)
		{
			p->position += p->velocity * dt;
			p->velocity.y += this->gravity * dt;
			p->velocity *= this->drag;
			p->color.a = 1.0f - ((this->life - p->life) / this->life);
		}
		p->life -= dt;
	}
}

void ParticleGenerator::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader_use(&this->shader);
	for (Particle particle : this->particles)
	{
		if (particle.life > 0)
		{
			shader_set_vector2f(&this->shader, "offset", particle.position);
			shader_set_vector4f(&this->shader, "color", particle.color);
			bind_texture(&this->texture);
			glBindVertexArray(this->vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CHECK_ERROR;
}