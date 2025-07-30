#ifndef ALIENCONTROL_H
#define ALIENCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/Alien.hpp"
#include "../view/LayerManager.hpp"
#include "../view/AlienView.hpp"

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

	// Gets the stored alien
	Alien& get();
	const Alien& get() const;

	// Resets cooldown for next shot
	void reset_shoot_timer(float intensity, std::mt19937& random);

	// Make alien shake
	void refresh_shake(float intensity, std::mt19937& random);

	// Stop shaking animation
	void reset_shake();

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

	// Starting and ending position of shake animation
	sf::Vector2f m_shake_start;
	sf::Vector2f m_shake_target;

	// Timer to check if old animation is complete and new one must begin
	float m_shake_timer;

	// Duration of shake animation
	float m_shake_duration;

	// Position and velocity during swerving
	sf::Vector2f m_swerve_position;
	sf::Vector2f m_swerve_velocity;

	// Position in grid to return to after swerving
	size_t m_swerve_target_column;
	size_t m_swerve_target_row;
};

#endif