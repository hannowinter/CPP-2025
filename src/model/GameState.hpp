#ifndef GAMESTATE_H
#define GAMESTATE_H

// Store state of game (score, level, number of lives left)
struct GameState
{
	size_t score = 0;
	size_t level = 1;
	size_t lives = 5;
	bool over = false;
};

#endif