#ifndef ALIENCONTROL_H
#define ALIENCONTROL_H

#include <SFML/Graphics.hpp>

#include "Control.hpp"
#include "../model/Alien.hpp"
#include "../view/LayerManager.hpp"
#include "../view/AlienView.hpp"

// Alien shake state
// Handles the aliens shaking smoothly.
// On each new cycle, the target offset and shake duration are randomized.
// The alien will slowly move towards the target offset and then start a new cycle.
struct ShakeState
{
	sf::Vector2f start{}; // shake start position
	sf::Vector2f target{}; // shake target position
	float timer{}; // elapsed time for the current shake cycle (in seconds), counts up to "duration" and then a new cycle begins
	float duration{}; // total duration for the current shake cycle (in seconds)

	// Updates the shake state.
	void update(float delta);
	// Checks if the current shake cycle finished.
	bool finished_cycle() const;
	// Starts a new randomized shake cycle.
	void new_cycle(float intensity, std::mt19937& random);
	// Starts a new shake cycle that moves towards { 0.0f, 0.0f }.
	void reset();

	// Calculates the alien's current offset produced by the shake.
	sf::Vector2f get_offset() const;
};

// Alien swerve state
// Handles the aliens' swerving maneuver.
// On each swerve maneuver, the alien will move towards the player (Mode::ATTACK), and then after reaching a
// certain threshold, turn around and realign with the grid (Mode::RETREAT).
struct SwerveState
{
	// Position and velocity during swerving
	sf::Vector2f position{};
	sf::Vector2f velocity{};

	// Position in grid to return to after swerving
	size_t target_column{};
	size_t target_row{};

	// Starts a new swerve maneuver.
	void start(
		size_t target_column, 
		size_t target_row, 
		sf::Vector2f init_position, 
		sf::Vector2f init_velocity
	);

	// Updates the process of attacking the player.
	// Returns "true" if the retreat threshold has been reached and the alien should start to retreat.
	bool update_attack(float delta, sf::Vector2f player_position);

	// Updates the process of retreating to the target destination.
	// Returns "true" if the retreating process has finished.
	bool update_retreat(float delta, float intensity, sf::Vector2f target);
};

// Control class for an individual alien.
class AlienControl : public Control
{
public:
	// Aliens can be in several distinct states
	enum Mode
	{
		GRID_ALIGNED, 	// Alien is aligned with the grid and shaking smoothly.
		ATTACK, 	// Alien is swerving out of the grid heading towards the player.
		RETREAT 	// Alien is retreating back to the grid after swerving out.
	};

	// Creates an AlienControl.
	AlienControl(Alien::Variant variant, sf::Vector2f grid_origin, size_t column, size_t row);

	// Abstract methods inherited from parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Gets the alien's current mode.
	Mode get_mode() const;

	// Gets a reference to the model object.
	Alien& get();
	const Alien& get() const;

	// Gets the swerve state.
	const SwerveState& get_swerve_state() const;

	// Resets cooldown for next shot.
	void reset_shoot_timer(float intensity, std::mt19937& random);

	// Initiates a new swerve maneuver, putting the alien into "ATTACK" mode.
	void start_swerve(sf::Vector2f init_velocity, size_t target_column, size_t target_row);

private:
	// Model
	Alien m_alien;

	// View
	AlienView m_alien_view;

	// Current mode of alien
	Mode m_mode;

	// Cooldown time remaining until next shot
	float m_shoot_timer;

	ShakeState m_shake_state;
	SwerveState m_swerve_state;
};

// Controls the alien grid as a whole.
// This class is responsible for spawning all aliens, moving the grid's origin point around and
// picking pairs of aliens to swerve at random times.
class AlienGridControl : public Control
{
public:
	// Possible modes of the grid
	enum Mode
	{
		SHIFT_RIGHT, 	// Alien grid is moving to the right until hitting the screen's border.
		SHIFT_LEFT, 	// Alien grid is moving to the left until hitting the screen's border.
		DESCEND 	// Alien grid is descending downwards for a certain amount of time.
	};

	// Creates an AlienGridControl.
	AlienGridControl();

	// Abstract methods inherited from parent
	void add_children(ControlList& controls) override;
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Updates the origin position, taking the screen's borders into account and changing the modes.
	void update_origin(float delta, float intensity, const ControlList& controls);

	// Gets the position of the grid's top-leftmost point
	sf::Vector2f origin() const;

	// Gets the base velocity the origin is moving with.
	sf::Vector2f origin_velocity() const;

	// Sets the mode.
	void set_mode(Mode new_mode);

	// Gets the mode.
	Mode get_mode() const;

	// Determines topmost and bottommost row in the grid of all aliens still alive.
	static std::pair<size_t, size_t> get_min_max_row(const ControlList& controls);
	// Determines leftmost and rightmost column in the grid of all aliens still alive.
	static std::pair<size_t, size_t> get_min_max_column(const ControlList& controls);

	// Gets y-coordinate of the bottommost alien's bottom.
	float get_bottom() const;

	// Resets the wait time until picking a new pair of aliens to swerve.
	void reset_swerve_timer(std::mt19937& random);

	// Picks two random distinct aliens and initiates a swerve.
	// If only one alien exists, only that one will swerve.
	void start_random_swerve(std::mt19937& random, const ControlList& controls) const;

private:
	// Current and previous mode of grid
	Mode m_mode;
	Mode m_prev_mode;

	// Origin of grid
	sf::Vector2f m_origin;

	// Timer measuring length of descend phase
	float m_descend_timer = 0.0f;

	// Cooldown until next swerve
	float m_swerve_timer = 0.0f;

	// Bottommost y-coordinate
	float m_bottom;
};

#endif