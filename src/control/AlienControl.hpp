#ifndef ALIENCONTROL_H
#define ALIENCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/Alien.hpp"
#include "../view/Layer.hpp"
#include "../view/AlienView.hpp"

// Controls an individual alien.
class AlienControl : public Control
{
public:
	enum Mode
	{
		GRID_ALIGNED, // alien is aligned with the grid and shaking smoothly
		SWERVE, // alien is swerving out of the grid heading towards the player
		RETREAT // alien is retreating back to the grid after swerving out
	};

	AlienControl(Alien::Variant variant, sf::Vector2f grid_origin, size_t column, size_t row);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

	// Gets the stored alien model.
	Alien& get();
	const Alien& get() const;

	// Resets the wait time until shooting again.
	void reset_shoot_timer(float intensity, std::mt19937& random);

	void refresh_shake(float intensity, std::mt19937& random);
	void reset_shake();

	// Initiates a swerve, putting the alien into `SWERVE` mode.
	void start_swerve(sf::Vector2f init_velocity, size_t target_column, size_t target_row);

private:
	Alien m_alien;
	AlienView m_alien_view;

	Mode m_mode;

	float m_shoot_timer;

	sf::Vector2f m_shake_start;
	sf::Vector2f m_shake_target;
	float m_shake_timer;
	float m_shake_duration;

	sf::Vector2f m_swerve_position;
	sf::Vector2f m_swerve_velocity;
	size_t m_swerve_target_column;
	size_t m_swerve_target_row;
};

#endif