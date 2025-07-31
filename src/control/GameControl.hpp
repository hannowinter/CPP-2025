#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <random>

#include "Controls.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"
#include "../view/LayerManager.hpp"
#include "../model/GameState.hpp"
#include "../view/HUDView.hpp"

// Controller for state of game
class GameControl : public Control
{
public:
	// Create GameController with controllers
	GameControl();

	// Abstract methods of parent class
	void add_children(ControlList& controls) override;
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	void increment_level();

	// Get pseudo-random number generator
	std::mt19937& random();

	// Get current game state
	GameState& state();
	const GameState& state() const;

	// Boolean to check if game is over
	bool is_over = false;

private:
	// Current state of the game
	GameState m_state;

	// HUD for score, lives, level, ...
	HUDView m_hud;

	// Pseudo-random number generator
	std::mt19937 m_random;
};

#endif