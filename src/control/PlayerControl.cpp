#include "PlayerControl.hpp"

#include "AlienBulletControl.hpp"
#include "AlienControl.hpp"
#include "AudioPlayer.hpp"
#include "GameControl.hpp"
#include "../model/Constants.hpp"
#include "PlayerBulletControl.hpp"
#include "../Util.hpp"

// Create PlayerController for Player at position
PlayerControl::PlayerControl(sf::Vector2f position) :
	m_player{ position },
	m_player_view{},
	m_shoot_cooldown{ 0.0f }
{

}

// Initialize controller
void PlayerControl::init(const ControlList& controls)
{
	// nothing to do here
}

// Execute relevant updates
void PlayerControl::update(const UpdateState& state) 
{
	// Decrement remaining cooldown
	m_shoot_cooldown -= state.delta;

	GameControl& game_control = *state.controls.get<GameControl>();

	// Check if player has been hit
	for (const auto& control : state.controls)
	{
		if (const AlienBulletControl* bullet = control->is<AlienBulletControl>())
		{
			if (overlaps(bullet->get().hitbox(), m_player.hitbox()))
			{
				// Delete bullet
				state.controls.remove(bullet);

				// Start hit animation
				m_player_view.hit_animation();

				// Decrement lives
				game_control.state().lives -= 1;

				// Play sound
				AudioPlayer::get().player_hit_bullet.play();
			}
		}
	}

	// Check if swerving alien has hit player
	for (const auto& control : state.controls)
	{
		if (AlienControl* alien = control->is<AlienControl>())
		{
			// Check if alien is swerving, close to player and may hit player
			if (alien->get_mode() == AlienControl::SWERVE &&
				!alien->get().has_hit_player &&
				(m_player.hitbox().position - alien->get().hitbox().position).length() <= 100.0f)
			{
				// Decrement lives and start hit animation
				m_player_view.hit_animation();
				game_control.state().lives -= 1;
				alien->get().has_hit_player = true;

				// Play sound
				AudioPlayer::get().player_hit_swerve.play();
			}
		}
	}

	// Update view
	m_player_view.update(state.delta);

	// Move player to the left if left arrow key pressed
	if (state.inputs.held_keys.contains(sf::Keyboard::Key::Left))
	{
		m_player.move(Player::LEFT, state.delta);
	}

	// Move player to the right if right arrow key pressed
	if (state.inputs.held_keys.contains(sf::Keyboard::Key::Right))
	{
		m_player.move(Player::RIGHT, state.delta);
	}

	// Shoot bullet if cooldown is over and space button is pressed
	if (
		m_shoot_cooldown <= 0.0f &&
		state.inputs.pressed_keys.contains(sf::Keyboard::Key::Space)
	)
	{
		// Create Controller for new bullet at position of player
		state.controls.add<PlayerBulletControl>(sf::Vector2f{
			m_player.hitbox().getCenter().x - constants::player_bullet::SIZE.x / 2.0f,
			m_player.hitbox().position.y - constants::player_bullet::SIZE.y
		});

		// Reset cooldown period
		m_shoot_cooldown = constants::player::SHOOT_COOLDOWN;
	}
}

// Draw player
void PlayerControl::draw(LayerManager& layers)
{
	m_player_view.draw(layers.get(LayerID::ACTORS), m_player);
}

// Get reference to Player
const Player& PlayerControl::get() const
{
	return m_player;
}

// Get reference to Player
Player& PlayerControl::get()
{
	return m_player;
}