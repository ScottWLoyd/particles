#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

#include "bo_math.h"
#include "game_object.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
};

#define MAX_LEVELS 5

class Game {
public:
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;
	std::vector<GameObject*> objects;
	
	Game(int width, int height);

	void process_input(GLfloat dt);
	void update(GLfloat dt);
	void render();

	void do_collisions();

private:	
	void init();
};



#endif