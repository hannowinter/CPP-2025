#ifndef ALIENCONTROL_H
#define ALIENCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/Alien.hpp"
#include "../view/LayerManager.hpp"
#include "../view/AlienView.hpp"

// handles the aliens shaking smoothly
struct ShakeState
{
	sf::Vector2f start{}; // shake start position
	sf::Vector2f target{}; // shake target position
	float timer{}; // elapsed time for the current shake cycle (in seconds), counts up to "duration" and then a new cycle begins
	float duration{}; // total duration for the current shake cycle (in seconds)

	// update the shake state
	void update(float delta);
	// check if the current shake cycle finished
	bool finished_cycle() const;
	// start a new randomized shake cycle
	void new_cycle(float intensity, std::mt19937& random);
	// start a new shake cycle that moves towards { 0.0f, 0.0f }
	void reset();

	// calculate the alien's offset produced by the shake
	sf::Vector2f get_offset() const;
};

// Controls an individual alien.
class AlienControl : public Control
{
public:
	// Aliens can be in several distinct states
	enum Mode
	{
		GRID_ALIGNED, 	// alien is aligned with the grid and shaking smoothly
		SWERVE, 	// alien is swerving out of the grid heading towards the player
		RETREAT 	// alien is retreating back to the grid after swerving out
	};

	// Create AlienController
	AlienControl(Alien::Variant variant, sf::Vector2f grid_origin, size_t column, size_t row);

	// Abstract methods inherited from parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Get mode of alien
	Mode get_mode() const;

	// Gets the stored alien
	Alien& get();
	const Alien& get() const;

	// Resets cooldown for next shot
	void reset_shoot_timer(float intensity, std::mt19937& random);

	// Initiates a swerve, putting the alien into "SWERVE" mode
	void start_swerve(sf::Vector2f init_velocity, size_t target_column, size_t target_row);

private:
	// Controlled Alien and AlienView
	Alien m_alien;
	AlienView m_alien_view;

	// Current mode of alien
	Mode m_mode;

	// Cooldown time remaining until next shot
	float m_shoot_timer;

	ShakeState m_shake_state;

	// Position and velocity during swerving
	sf::Vector2f m_swerve_position;
	sf::Vector2f m_swerve_velocity;

	// Position in grid to return to after swerving
	size_t m_swerve_target_column;
	size_t m_swerve_target_row;
};

#endif