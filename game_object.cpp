#include "sprite_renderer.h"
#include "game_object.h"

GameObject::GameObject()
{
	this->color = Vec3{ 0 };
	this->position = Vec2{ 0 };
	this->size = Vec2{ 0 };
	this->velocity = Vec2{ 0 };
	this->rotation = 0;
}

GameObject::GameObject(Vec2 position, Vec2 size, Vec2 velocity)
	: position(position), size(size), velocity(velocity)
{	
}

GameObject::~GameObject()
{
}

void GameObject::update(float dt)
{
}

#if 0
static CollisionResult check_collision(GameObject* a, GameObject* b)
{
	// If one is ball and one is a block, make sure a is the ball
	if (a->type != b->type && b->type == GameObject_Ball)
	{
		GameObject* temp = a;
		a = b;
		b = temp;
	}
	if (a->type == GameObject_Ball)
	{
		Vec2 center = a->position + a->ball_radius;
		Vec2 half_extents = Vec2{ b->size.x / 2.0f, b->size.y / 2.0f };
		Vec2 aabb_center = Vec2{ b->position.x + half_extents.x, b->position.y + half_extents.y };
		Vec2 difference = center - aabb_center;
		Vec2 clamped = glm::clamp(difference, -half_extents, half_extents);
		Vec2 closest = aabb_center + clamped;
		difference = closest - center;
		if (glm::length(difference) <= a->ball_radius)
		{
			return CollisionResult{ true, vector_direction(difference), difference };
		}
		else
		{
			return CollisionResult{ false, DirectionUp, Vec2{0} };
		}
	}
	else
	{
		assert(a->type == GameObject_Brick || a->type == GameObject_Player);
		assert(b->type == GameObject_Brick || b->type == GameObject_Player);
		bool x_axis = a->position.x + a->size.x >= b->position.x &&
			b->position.x + b->size.x >= a->position.x;
		bool y_axis = a->position.y + a->size.y >= b->position.y &&
			b->position.y + b->size.y >= b->position.y;

		// TODO(scott): actually calculate this
		if (x_axis && y_axis)
		{
			return CollisionResult{ true, DirectionUp, Vec2{0} };
		}
		else
		{
			return CollisionResult{ false, DirectionUp, Vec2{ 0 } };
		}
	}
}
#endif