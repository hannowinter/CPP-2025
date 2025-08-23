#ifndef ALIENCONTROL_H
#define ALIENCONTROL_H

#include <SFML/Graphics.hpp>

#include "Control.hpp"
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

// handles the aliens' swerving maneuver
struct SwerveState
{
	// Position and velocity during swerving
	sf::Vector2f position{};
	sf::Vector2f velocity{};

	// Position in grid to return to after swerving
	size_t target_column{};
	size_t target_row{};

	void start(
		size_t target_column, 
		size_t target_row, 
		sf::Vector2f init_position, 
		sf::Vector2f init_velocity
	);

	// update the process of swerving towards the player
	// returns "true" if the retreat threshold has been reached and the alien should start to retreat
	bool update_swerve(float delta, sf::Vector2f player_position);

	// update the process of retreating to the target destination
	// returns "true" if the retreating process has finished
	bool update_retreat(float delta, float intensity, sf::Vector2f target);
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

	// Gets the swerve state
	const SwerveState& get_swerve_state() const;

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

	SwerveState m_swerve_state;
};

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

	// Determine topmost and bottommost row in the grid of all aliens still alive.
	static std::pair<size_t, size_t> get_min_max_row(const ControlList& controls);
	// Determine leftmost and rightmost column in the grid of all aliens still alive.
	static std::pair<size_t, size_t> get_min_max_column(const ControlList& controls);

	// Get y-coordinate of bottommost alien
	float get_bottom();

	// Resets the wait time until picking a new pair of aliens to swerve.
	void reset_swerve_timer(std::mt19937& random);

	// Picks two random distinct aliens and initiates a swerve.
	// If only one alien exists, only that one will swerve.
	static void start_random_swerve(std::mt19937& random, const ControlList& controls, sf::Vector2f velocity);

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