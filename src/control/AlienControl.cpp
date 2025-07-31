#include "AlienControl.hpp"
#include "AlienGridControl.hpp"
#include "AlienBulletControl.hpp"
#include "PlayerBulletControl.hpp"
#include "GameControl.hpp"
#include "PlayerControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

// Create new AlienController
AlienControl::AlienControl(Alien::Variant variant, sf::Vector2f grid_origin, size_t column, size_t row) :
	m_alien{ variant, grid_origin, column, row },
	m_alien_view{ variant },
	m_mode{ GRID_ALIGNED },
	m_shoot_timer{},
	m_shake_start{},
	m_shake_target{},
	m_shake_timer{},
	m_shake_duration{},
	m_swerve_position{},
	m_swerve_velocity{},
	m_swerve_target_column{},
	m_swerve_target_row{}
{

}

// Initialize AlienController
void AlienControl::init(const ControlList& controls)
{
	// Get GameController
	GameControl& game_control = *controls.get<GameControl>();

	// Randomize alien animation, shooting cooldown and shaking animation
	m_alien_view.randomize_animation(game_control.random());
	reset_shoot_timer(constants::alien_grid::MIN_INTENSITY, game_control.random());
	refresh_shake(constants::alien_grid::MIN_INTENSITY, game_control.random());
}

// Execute all relevant updates
void AlienControl::update(const UpdateState& state)
{
	// Get AlienGridControl and GameControl
	const AlienGridControl& alien_grid_control = *state.controls.get<AlienGridControl>();
	GameControl& game_control = *state.controls.get<GameControl>();

	// Check for collisions with bullets
	for (const auto& control : state.controls)
	{
		if (const PlayerBulletControl* bullet = control->is<PlayerBulletControl>())
		{
			if (overlaps(bullet->get().hitbox(), m_alien.hitbox()))
			{ // collision detected, despawn alien and bullet
				state.controls.remove(this);
				state.controls.remove(bullet);

				// Increment score
				game_control.state().score += 50;
			}
		}
	}

	// Update view (animations are sped up by a factor of intensity())
	m_alien_view.update(state.delta * alien_grid_control.intensity());

	// Check if alien needs to shoot
	m_shoot_timer -= state.delta;
	if (m_shoot_timer < 0.0f)
	{
		// Create Bullet at position of alien
		state.controls.add<AlienBulletControl>(sf::Vector2f{
			m_alien.hitbox().getCenter().x - constants::alien_bullet::SIZE.x / 2.0f,
			m_alien.hitbox().position.y + constants::alien::SIZE.y
		});

		// Reset cooldown
		reset_shoot_timer(alien_grid_control.intensity(), game_control.random());
	}

	// update alien depending on current mode
	switch (m_mode)
	{
	case GRID_ALIGNED:
	{
		// check if alien needs to initiate a new shake iteration
		m_shake_timer += state.delta;
		if (m_shake_timer >= m_shake_duration)
		{
			refresh_shake(
				alien_grid_control.intensity(),
				game_control.random()
			);
		}

		// calculate the shake offset
		constexpr static auto lerp = []<typename T, typename R>(const T& v1, const T& v2, const R& r)
		{ // std::lerp does not work with sf::Vector2f unfortunately
			return v1 * (R{ 1 } - r) + v2 * r;
		};

		float shake_ratio = m_shake_timer / m_shake_duration;
		sf::Vector2f shake_offset = lerp(m_shake_start, m_shake_target, shake_ratio);
		
		m_alien.set_position(alien_grid_control.origin() + m_alien.grid_offset() + shake_offset);
		break;
	}
	case SWERVE:
	{
		// Get Player reference and position of player
		const PlayerControl& player = *state.controls.get<PlayerControl>();
		sf::Vector2f player_position = player.get().hitbox().position;

		const float MAX_SPEED = constants::alien::SWERVE_MAX_SPEED;

		// Calculate acceleration vector
		sf::Vector2f acceleration = 
			(player_position - m_swerve_position).normalized() * 
			constants::alien::SWERVE_ACCELERATION;

		// Increment alien velocity
		m_swerve_velocity += acceleration * state.delta;

		// Limit velocity to MAX_SPEED
		if (m_swerve_velocity.length() > MAX_SPEED)
			m_swerve_velocity = m_swerve_velocity.normalized() * MAX_SPEED;

		// Change swerving position
		m_swerve_position += m_swerve_velocity * state.delta;

		// Make alien retreat if below threshold height
		if (m_swerve_position.y >= constants::VIEW_HEIGHT - constants::alien::RETREAT_THRESHOLD)
			m_mode = RETREAT;

		// Set alien position to swerving position
		m_alien.set_position(m_swerve_position);
		break;
	}
	case RETREAT:
	{
		// Set new position of alien within grid
		m_alien.set_column_row(m_swerve_target_column, m_swerve_target_row);
		sf::Vector2f target = alien_grid_control.origin() + m_alien.grid_offset();

		m_alien.has_hit_player = false;

		// Calculate distance from grid position, acceleration and speed
		float dist = (m_swerve_position - target).length();
		float acceleration_length = constants::alien::RETREAT_ACCELERATION;
		float max_speed = std::max(
			constants::alien_grid::SHIFT_SPEED * alien_grid_control.intensity() * 1.5f,
			constants::alien::RETREAT_MIN_SPEED
		);

		// If the remaining distance is short, make alien part of grid again and break
		if (dist <= 100.0f)
		{
			sf::Vector2f dist = target - m_swerve_position;
			if (max_speed * state.delta < dist.length())
			{
				m_swerve_velocity = dist.normalized() * max_speed;
			}
			else
			{
				m_alien.set_position(target);
				m_mode = GRID_ALIGNED;
				break;
			}
		}

		// Otherwise determine velocity
		else
		{
			sf::Vector2f acceleration =
				(target - m_swerve_position).normalized() *
				acceleration_length;
			m_swerve_velocity += acceleration * state.delta;
			if (m_swerve_velocity.length() > max_speed)
				m_swerve_velocity = m_swerve_velocity.normalized() * max_speed;
		}

		// Determine and set new position
		m_swerve_position += m_swerve_velocity * state.delta;
		m_alien.set_position(m_swerve_position);
		break;
	}
	}
}

// Draw alien on scene
void AlienControl::draw(LayerManager& layers)
{
	m_alien_view.draw(layers.get(LayerID::ACTORS), m_alien);
}

// Get mode of alien
AlienControl::Mode AlienControl::get_mode() const
{
	return m_mode;
}


// Get reference to alien
Alien& AlienControl::get()
{
	return m_alien;
}

// Get reference to alien
const Alien& AlienControl::get() const
{
	return m_alien;
}

// Rest cooldown for next shot
void AlienControl::reset_shoot_timer(float intensity, std::mt19937& random)
{
	std::uniform_real_distribution<float> dist{
		0.0f,
		constants::alien::MAX_SHOOT_TIME / intensity
	};

	m_shoot_timer = dist(random);
}

// Make alien shake
void AlienControl::refresh_shake(float intensity, std::mt19937& random)
{
	intensity = std::pow(intensity, constants::alien::SHAKE_INTENSITY_EXPONENT);

	std::uniform_real_distribution<float> target_dist{
		constants::alien::SHAKE_MIN_TARGET_OFFSET,
		constants::alien::SHAKE_MAX_TARGET_OFFSET * intensity
	};

	std::uniform_real_distribution<float> duration_dist{
		constants::alien::SHAKE_MIN_DURATION,
		constants::alien::SHAKE_MAX_DURATION
	};

	// Set starting position to current position after finishing shaking
	m_shake_start = m_shake_target;

	// Calculate new target positon for shaking animation
	m_shake_target = { target_dist(random), target_dist(random) };

	// Determine duration and reset timer
	m_shake_duration = duration_dist(random) / intensity;
	m_shake_timer = 0.0f;
}

// Stop shaking animation
void AlienControl::reset_shake()
{
	m_shake_start = { 0.0f, 0.0f };
	m_shake_target = { 0.0f, 0.0f };
	m_shake_timer = 0.0f;
}

// Make alien swerve
void AlienControl::start_swerve(sf::Vector2f init_velocity, size_t target_column, size_t target_row)
{
	m_mode = SWERVE;
	m_swerve_position = m_alien.hitbox().position;
	m_swerve_velocity = init_velocity;
	reset_shake();
	m_swerve_target_column = target_column;
	m_swerve_target_row = target_row;
}