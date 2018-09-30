#ifndef _BO_MATH_H_
#define _BO_MATH_H_

#define _USE_MATH_DEFINES 
#include <math.h>
#include <glm.hpp>

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;
typedef glm::mat4 Mat4;

enum ScreenDirection {
	DirectionUp,
	DirectionLeft,
	DirectionDown,
	DirectionRight,
};

static ScreenDirection vector_direction(Vec2 v)
{
	Vec2 compass[] = {
		{ 0, 1 }, // Up
		{ -1, 0 }, // Left
		{ 0, -1 }, // Down
		{ 1, 0 }, // Right
	};

	float max = 0;
	int best_match = -1;
	for (int i = 0; i < 4; i++)
	{
		float dot = glm::dot(v, compass[i]);
		if (dot > max)
		{
			max = dot;
			best_match = i;
		}
	}
	return (ScreenDirection)best_match;
}

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define CLAMP(v, min, max) (MAX(min, MIN(max, v)))
#define LERP(min, v, max) ((min) + (v)*((max)-(min)))

#define RADIANS(d) ((d)*M_PI/180)
#define DEGREES(r) ((r)*180/M_PI)

Vec4 lerp(Vec4 a, Vec4 b, float t)
{
	Vec4 result;
	result.x = LERP(a.x, t, b.x);
	result.y = LERP(a.y, t, b.y);
	result.z = LERP(a.z, t, b.z);
	result.w = LERP(a.w, t, b.w);
	return result;
}

#endif
