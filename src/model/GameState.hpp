#ifndef GAMESTATE_H
#define GAMESTATE_H

// Store state of game (score, level, number of lives left)
struct GameState
{
	size_t score = 2000;
	size_t level = 5;
	size_t lives = 3;
};

#endif