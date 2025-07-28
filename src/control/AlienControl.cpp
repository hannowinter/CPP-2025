#include "AlienControl.hpp"
#include "AlienGridControl.hpp"
#include "AlienBulletControl.hpp"
#include "PlayerBulletControl.hpp"
#include "GameControl.hpp"
#include "PlayerControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

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

void AlienControl::init(const ControlList& controls)
{
	GameControl& game_control = *controls.get<GameControl>();
	m_alien_view.randomize_animation(game_control.random());
	reset_shoot_timer(constants::alien_grid::MIN_INTENSITY, game_control.random());
	refresh_shake(constants::alien_grid::MIN_INTENSITY, game_control.random());
}

void AlienControl::update(const UpdateState& state)
{
	const AlienGridControl& alien_grid_control = *state.controls.get<AlienGridControl>();
	GameControl& game_control = *state.controls.get<GameControl>();

	// check for collisions with bullets
	for (const auto& control : state.controls)
	{
		if (const PlayerBulletControl* bullet = control->is<PlayerBulletControl>())
		{
			if (overlaps(bullet->get().hitbox(), m_alien.hitbox()))
			{ // collision detected, despawn alien and bullet
				state.controls.remove(this);
				state.controls.remove(bullet);
			}
		}
	}

	m_alien_view.update(state.delta * alien_grid_control.intensity());

	// check if alien needs to shoot
	m_shoot_timer -= state.delta;
	if (m_shoot_timer < 0.0f)
	{
		state.controls.add<AlienBulletControl>(sf::Vector2f{
			m_alien.hitbox().getCenter().x - constants::alien_bullet::SIZE.x / 2.0f,
			m_alien.hitbox().position.y + constants::alien::SIZE.y
		});
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
		const PlayerControl& player = *state.controls.get<PlayerControl>();
		sf::Vector2f player_position = player.get().hitbox().position;
		const float MAX_SPEED = constants::alien::SWERVE_MAX_SPEED;

		sf::Vector2f acceleration = 
			(player_position - m_swerve_position).normalized() * 
			constants::alien::SWERVE_ACCELERATION;
		m_swerve_velocity += acceleration * state.delta;
		if (m_swerve_velocity.length() > MAX_SPEED)
			m_swerve_velocity = m_swerve_velocity.normalized() * MAX_SPEED;
		m_swerve_position += m_swerve_velocity * state.delta;

		if (m_swerve_position.y >= constants::VIEW_HEIGHT - constants::alien::RETREAT_THRESHOLD)
			m_mode = RETREAT;

		m_alien.set_position(m_swerve_position);
		break;
	}
	case RETREAT:
	{
		m_alien.set_column_row(m_swerve_target_column, m_swerve_target_row);
		sf::Vector2f target = alien_grid_control.origin() + m_alien.grid_offset();
		float dist = (m_swerve_position - target).length();
		float acceleration_length = constants::alien::RETREAT_ACCELERATION;
		float max_speed = std::max(
			constants::alien_grid::SHIFT_SPEED * alien_grid_control.intensity() * 1.5f,
			constants::alien::RETREAT_MIN_SPEED
		);
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
		else
		{
			sf::Vector2f acceleration =
				(target - m_swerve_position).normalized() *
				acceleration_length;
			m_swerve_velocity += acceleration * state.delta;
			if (m_swerve_velocity.length() > max_speed)
				m_swerve_velocity = m_swerve_velocity.normalized() * max_speed;
		}
		m_swerve_position += m_swerve_velocity * state.delta;
		m_alien.set_position(m_swerve_position);
		break;
	}
	}
}

void AlienControl::draw(Layer& layer)
{
	m_alien_view.draw(layer, m_alien);
}

Alien& AlienControl::get()
{
	return m_alien;
}

const Alien& AlienControl::get() const
{
	return m_alien;
}

void AlienControl::reset_shoot_timer(float intensity, std::mt19937& random)
{
	std::uniform_real_distribution<float> dist{
		0.0f,
		constants::alien::MAX_SHOOT_TIME / intensity
	};
	m_shoot_timer = dist(random);
}

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

	m_shake_start = m_shake_target;
	m_shake_target = { target_dist(random), target_dist(random) };
	m_shake_duration = duration_dist(random) / intensity;
	m_shake_timer = 0.0f;
}

void AlienControl::reset_shake()
{
	m_shake_start = { 0.0f, 0.0f };
	m_shake_target = { 0.0f, 0.0f };
	m_shake_timer = 0.0f;
}

void AlienControl::start_swerve(sf::Vector2f init_velocity, size_t target_column, size_t target_row)
{
	m_mode = SWERVE;
	m_swerve_position = m_alien.hitbox().position;
	m_swerve_velocity = init_velocity;
	reset_shake();
	m_swerve_target_column = target_column;
	m_swerve_target_row = target_row;
}