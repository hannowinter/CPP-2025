#include "AlienControl.hpp"
#include "AlienGridControl.hpp"
#include "AlienBulletControl.hpp"
#include "AudioPlayer.hpp"
#include "BombControl.hpp"
#include "PlayerBulletControl.hpp"
#include "GameControl.hpp"
#include "LaserControl.hpp"
#include "PlayerControl.hpp"
#include "SmokeControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

void ShakeState::update(float delta)
{
	timer += delta;
	if (timer > duration)
		timer = duration;
}

bool ShakeState::finished_cycle() const
{
	return timer >= duration;
}

void ShakeState::reset()
{
	start = get_offset();
	target = { 0.0f, 0.0f };
	timer = 0.0f;
	duration = constants::alien::SHAKE_MIN_DURATION;
}

void ShakeState::new_cycle(float intensity, std::mt19937& random)
{
	std::uniform_real_distribution<float> target_dist{
		constants::alien::SHAKE_MIN_TARGET_OFFSET,
		constants::alien::SHAKE_MAX_TARGET_OFFSET * intensity
	};

	std::uniform_real_distribution<float> duration_dist{
		constants::alien::SHAKE_MIN_DURATION / intensity,
		constants::alien::SHAKE_MAX_DURATION / intensity
	};

	// Set starting position to what the previous target position was
	start = target;

	// Calculate new target positon for shaking animation
	target = { target_dist(random), target_dist(random) };

	// Determine duration and reset timer
	duration = duration_dist(random);
	timer = 0.0f;
}

sf::Vector2f ShakeState::get_offset() const
{
	float ratio = timer / duration;
	return lerp(start, target, ratio);
}

void SwerveState::start(
	size_t target_column,
	size_t target_row,
	sf::Vector2f init_position,
	sf::Vector2f init_velocity
) 
{
	position = init_position;
	velocity = init_velocity;
	this->target_column = target_column;
	this->target_row = target_row;
}

bool SwerveState::update_swerve(float delta, sf::Vector2f player_position)
{
	constexpr float MAX_SPEED = constants::alien::SWERVE_MAX_SPEED;

	// Calculate acceleration vector
	sf::Vector2f acceleration =
		(player_position - position).normalized() *
		constants::alien::SWERVE_ACCELERATION;

	// Apply acceleration to velocity
	velocity += acceleration * delta;

	// Limit velocity to MAX_SPEED
	if (velocity.length() > MAX_SPEED)
		velocity = velocity.normalized() * MAX_SPEED;

	// Apply velocity to position
	position += velocity * delta;

	// Check if alien should retreat
	return position.y >= constants::VIEW_HEIGHT - constants::alien::RETREAT_THRESHOLD;
}

bool SwerveState::update_retreat(float delta, float intensity, sf::Vector2f target)
{
	// Calculate distance to target
	sf::Vector2 dist = target - position;

	// Calculate max speed
	const float MAX_SPEED = std::max(
		constants::alien_grid::SHIFT_SPEED * intensity * 1.5f, // we want
		// the alien to move a bit faster than the grid to allow it to re-align more easily

		constants::alien::RETREAT_MIN_SPEED
	);

	// If the remaining distance is short, switch from acceleration-based movement to
	// fixed-speed movement (prevents overshooting the target)
	if (dist.length() <= 100.0f)
	{
		if (MAX_SPEED * delta < dist.length())
		{
			velocity = dist.normalized() * MAX_SPEED;
		}
		else // the next step of movement would overshoot, so we finish the retreat
		{
			position = target;
			return true;
		}
	}

	// Otherwise continue with acceleration-based movement
	else
	{
		sf::Vector2f acceleration =
			(target - position).normalized() *
			constants::alien::RETREAT_ACCELERATION;

		// Apply acceleration to velocity
		velocity += acceleration * delta;

		// Limit velocity to MAX_SPEED
		if (velocity.length() > MAX_SPEED)
			velocity = velocity.normalized() * MAX_SPEED;
	}

	// Apply velocity to position
	position += velocity * delta;
	return false;
}

// Create new AlienController
AlienControl::AlienControl(Alien::Variant variant, sf::Vector2f grid_origin, size_t column, size_t row) :
	m_alien{ variant, grid_origin, column, row },
	m_alien_view{ variant },
	m_mode{ GRID_ALIGNED },
	m_shoot_timer{},
	m_shake_state{},
	m_swerve_state{}
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
	m_shake_state.new_cycle(constants::alien_grid::MIN_INTENSITY, game_control.random());
}

// Execute all relevant updates
void AlienControl::update(const UpdateState& state)
{
	// Get AlienGridControl and GameControl
	const AlienGridControl& alien_grid_control = *state.controls.get<AlienGridControl>();
	GameControl& game_control = *state.controls.get<GameControl>();
	float intensity = game_control.intensity();

	// Check for collisions
	for (const auto& control : state.controls)
	{
		// Collisions with bullets
		if (const PlayerBulletControl* bullet = control->is<PlayerBulletControl>())
		{
			if (overlaps(bullet->get().hitbox(), m_alien.hitbox()))
			{
				// Show smoke texture
				state.controls.request_add<SmokeControl>(m_alien.hitbox().position);

				// Despawn alien and bullet
				state.controls.request_remove(this);
				state.controls.request_remove(bullet);

				// Play hit sound
				AudioPlayer::get().alien_hit.play();

				// Increment score
				game_control.state().score += 50;
			}
		}

		// Collisions with lasers
		if (const LaserControl* laser = control->is<LaserControl>())
		{
			if (overlaps(laser->get().hitbox(), m_alien.hitbox()))
			{
				// Show smoke texture
				state.controls.request_add<SmokeControl>(m_alien.hitbox().position);

				// Despawn alien
				state.controls.request_remove(this);

				// Play hit sound
				AudioPlayer::get().alien_hit.play();

				// Increment score
				game_control.state().score += 100;
			}
		}

		// Collisions with bombs
		if (BombControl* bomb = control->is<BombControl>())
		{
			// Check if bomb explodes on contact with this alien
			if (overlaps(bomb->get().hitbox(), m_alien.hitbox()) && !bomb->has_exploded())
			{
				// Show smoke texture
				state.controls.request_add<SmokeControl>(m_alien.hitbox().position);

				// Despawn alien
				state.controls.request_remove(this);

				// Play hit and explosion sound, make bomb explode
				AudioPlayer::get().alien_hit.play();
				AudioPlayer::get().explosion.play();
				bomb->explode();

				// Increment score
				game_control.state().score += 150;
			}
			// Check if alien is within explosion
			else if (overlaps(bomb->get().hitbox(), m_alien.hitbox()) && bomb->has_exploded())
			{
				// Show smoke texture
				state.controls.request_add<SmokeControl>(m_alien.hitbox().position);

				// Despawn alien
				state.controls.request_remove(this);

				// Play hit sound
				AudioPlayer::get().alien_hit.play();

				// Increment score
				game_control.state().score += 150;
			}
		}
	}

	// Update view (animations are sped up by a factor of intensity())
	m_alien_view.update(state.delta * intensity);

	// Check if alien needs to shoot
	m_shoot_timer -= state.delta;
	if (m_shoot_timer < 0.0f)
	{
		// Create Bullet at position of alien
		state.controls.request_add<AlienBulletControl>(sf::Vector2f{
			m_alien.hitbox().getCenter().x - constants::alien_bullet::SIZE.x / 2.0f,
			m_alien.hitbox().position.y + constants::alien::SIZE.y
		});

		// Reset cooldown
		reset_shoot_timer(intensity, game_control.random());
	}

	// update the shake state
	m_shake_state.update(state.delta);

	sf::Vector2f new_position;

	// update alien depending on current mode
	switch (m_mode)
	{
	case GRID_ALIGNED:
	{
		// check if new cycle needs to be initiated
		if (m_shake_state.finished_cycle())
		{			
			// weaken the intensity a bit to make the shaking not scale as strongly
			float intensity_weakened = std::pow(intensity, constants::alien::SHAKE_INTENSITY_EXPONENT);
			
			m_shake_state.new_cycle(
				intensity_weakened,
				game_control.random()
			);
		}

		new_position = alien_grid_control.origin() + m_alien.grid_offset();
		break;
	}
	case SWERVE:
	{
		// Get Player reference and position
		const PlayerControl& player = *state.controls.get<PlayerControl>();
		sf::Vector2f player_position = player.get().hitbox().position;

		bool should_retreat = m_swerve_state.update_swerve(state.delta, player_position);
		if (should_retreat)
			m_mode = RETREAT;

		new_position = m_swerve_state.position;
		break;
	}
	case RETREAT:
	{
		// Set new row and column of alien within grid
		m_alien.set_column_row(m_swerve_state.target_column, m_swerve_state.target_row);

		// Get the retreat target position
		sf::Vector2f target = alien_grid_control.origin() + m_alien.grid_offset();

		m_alien.has_hit_player = false;

		bool finished = m_swerve_state.update_retreat(state.delta, intensity, target);
		if (finished)
			m_mode = GRID_ALIGNED;

		new_position = m_swerve_state.position;
		break;
	}
	}

	sf::Vector2f shake_offset = m_shake_state.get_offset();
	m_alien.set_position(new_position + shake_offset);
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

const SwerveState& AlienControl::get_swerve_state() const
{
	return m_swerve_state;
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

// Make alien swerve
void AlienControl::start_swerve(sf::Vector2f init_velocity, size_t target_column, size_t target_row)
{
	m_mode = SWERVE;
	m_shake_state.reset();
	m_swerve_state.start(target_column, target_row, m_alien.hitbox().position, init_velocity);
}