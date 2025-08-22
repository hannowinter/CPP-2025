#ifndef ALIENGRIDCONTROL_H
#define ALIENGRIDCONTROL_H

#include <random>

#include "Controls.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"

// Controls the alien grid as a whole.
// This class is responsible for spawning all aliens, moving the grid's origin point around,
// determining the intensity and picking pairs of aliens to swerve at random times.
class AlienGridControl : public Control
{
public:
	// Possible states of the grid
	enum Mode
	{
		SHIFT_RIGHT, 	// alien grid is moving to the right until hitting the screen's border
		SHIFT_LEFT, 	// alien grid is moving to the left until hitting the screen's border
		DESCEND 	// alien grid is descending downwards for a certain amount of time
	};

	// Create AlienGridControl
	AlienGridControl();

	// Abstract methods inherited from parent
	void add_children(ControlList& controls) override;
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Returns the position of the grid's top-leftmost point
	sf::Vector2f origin() const;

	// Sets the mode.
	void set_mode(Mode new_mode);

	// Get y-coordinate of bottommost alien
	float get_bottom();

	// Resets the wait time until picking a new pair of aliens to swerve.
	void reset_swerve_timer(std::mt19937& random);

private:
	// Surrogate for difficulty
	float m_intensity = 1.0f;

	// Current and previous mode of grid
	Mode m_mode;
	Mode m_prev_mode;

	// Origin of grid
	sf::Vector2f m_origin;

	// Timer measuring length of descend phase
	float m_descend_timer = 0.0f;

	// Cooldown until next swerve
	float m_swerve_timer = 0.0f;

	// Bottommost pixel
	float m_bottom;
};

#endif