#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <random>

#include "Control.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"
#include "../view/LayerManager.hpp"
#include "../model/GameState.hpp"
#include "../view/HUDView.hpp"
#include "../view/BackgroundView.hpp"

// Control class managing the state of the game
class GameControl : public Control
{
public:
	// Creates a new GameControl.
	GameControl();

	// Abstract methods of parent class
	void add_children(ControlList& controls) override;
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Increments current level.
	void increment_level();
	// Resets current level.
	void reset_game();

	// Returns the current intensity, which depends on the current level.
	// The intensity controls the difficulty by influencing move speed, timers and shoot frequency.
	float intensity() const;

	// Gets the pseudo-random number generator instance.
	std::mt19937& random();

	// Gets current game state.
	GameState& state();
	const GameState& state() const;

private:
	// Current state of the game
	GameState m_state;

	// HUD for score, lives, level, ...
	HUDView m_hud;

	// Starry background
	BackgroundView m_background;

	// Pseudo-random number generator
	std::mt19937 m_random;

	// Boolean indicating whether GameOver screen has been shown to prevent audio bugs
	bool m_gameover_shown = false;

	// Boolean indicating whether Level Won screen has been shown to prevent audio bugs
	bool m_victory_shown = false;
};

#endif