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

	float particle_quad[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f, 
		0.5f, 0.5f, 0.0f,
	};

	glGenBuffers(1, &this->billboard_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->billboard_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glGenBuffers(1, &this->pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->pos_vbo);
	this->amount = this->rate * this->life;
	// Initialize with empty buffer - it will be filled each frame
	glBufferData(GL_ARRAY_BUFFER, this->amount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &this->color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->color_vbo);
	// Initialize with empty buffer - it will be filled each frame
	glBufferData(GL_ARRAY_BUFFER, this->amount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	for (uint32_t i = 0; i < this->amount; i++)
	{
		Particle p;
		this->particles.push_back(p);
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
	particle->scale = 1;
	particle->life = this->life;
	float total_angle = RADIANS(this->rotation + theta);
	particle->velocity = speed * Vec2{ cos(total_angle), -sin(total_angle) };
}

void ParticleGenerator::update(float dt)
{
#if 0
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
#else 
	uint32_t new_particles = (uint32_t)(dt * this->rate);
	for (uint32_t i = 0; i < new_particles; i++)
	{
		int unused_particle = this->first_unused_particle();
		this->respawn_particle(&this->particles[unused_particle]);
	}
	this->particle_position_size_data.clear();
	for (uint32_t i = 0; i < this->amount; i++)
	{
		Particle* p = &this->particles[i];
		if (p->life > 0)
		{
			p->velocity.y += this->gravity * dt;
			p->velocity *= this->drag;
			p->position += p->velocity * dt;
			p->color.a = 1.0f - ((this->life - p->life) / this->life);

			this->particle_position_size_data.push_back(Vec4{ p->position.x, p->position.y, 0, p->scale });
			this->particle_color_data.push_back(p->color);
		}
		p->life -= dt;
	}
#endif
}

void ParticleGenerator::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader_use(&this->shader);
	glBindBuffer(GL_ARRAY_BUFFER, this->pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, this->amount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->amount * 4 * sizeof(GLfloat), this->particle_position_size_data.data());

	glBindBuffer(GL_ARRAY_BUFFER, this->color_vbo);
	glBufferData(GL_ARRAY_BUFFER, this->amount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->amount * 4 * sizeof(GLfloat), this->particle_color_data.data());

	// First attribute buffer - vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->billboard_vbo);
	glVertexAttribPointer(
		0, // attribute - must match layout in shader
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// Second attribute buffer - positions of particles
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->pos_vbo);
	glVertexAttribPointer(
		1, // attribute - must match layout in shader
		4, // size - x + y + z + size 
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	// Third attribute buffer - vertex colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->color_vbo);
	glVertexAttribPointer(
		2, 
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0);	// particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1);	// positions : one per quad (its center) -> 1
	glVertexAttribDivisor(2, 1);	// color : one per quad -> 1

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->amount);
#if 0
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
#endif
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CHECK_ERROR;
}