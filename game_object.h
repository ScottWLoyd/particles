#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "bo_math.h"
#include "texture.h"
#include "sprite_renderer.h"
#include "game.h"

struct CollisionResult {
	bool collided;
	ScreenDirection direction;
	Vec2 difference;
};

enum GameObjectType {
	GameObject_None,
	GameObject_Brick,
	GameObject_Player,
	GameObject_Ball,
};

class GameObject {
public:
	Vec2 position;
	Vec2 size;
	Vec2 velocity;
	Vec3 color;
	float rotation;

	GameObject();
	GameObject(Vec2 position, Vec2 size, Vec2 velocity);
	~GameObject();

	virtual void update(float dt);
	virtual void draw() = 0;
};

CollisionResult check_collision(GameObject* a, GameObject* b);

#endif