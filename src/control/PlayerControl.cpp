#include "PlayerControl.hpp"

#include "AlienBulletControl.hpp"
#include "AlienControl.hpp"
#include "AudioPlayer.hpp"
#include "BombControl.hpp"
#include "GameControl.hpp"
#include "LaserControl.hpp"
#include "../model/Constants.hpp"
#include "PlayerBulletControl.hpp"
#include "UpgradeControl.hpp"
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
	// Get GameController
	GameControl& game_control = *state.controls.get<GameControl>();

	// Prevent automatic shooting if GameOver- or Win-Screen is shown
	if (game_control.state().over || game_control.state().level_won)
		m_shoot_cooldown = INFINITY;

	// Decrement remaining cooldown
	m_shoot_cooldown -= state.delta;

	// Check if player has been hit
	for (const auto& control : state.controls)
	{
		// Check if player has been hit by alien bullet
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

		// Check if player has been hit by swerving alien
		if (AlienControl* alien = control->is<AlienControl>())
		{
			// Check if alien is swerving, close to player and may hit player
			if (alien->get_mode() == AlienControl::SWERVE &&
				!alien->get().has_hit_player &&
				(m_player.hitbox().position - alien->get().hitbox().position).length() <= 90.0f)
			{
				// Decrement lives and start hit animation
				m_player_view.hit_animation();
				game_control.state().lives -= 1;
				alien->get().has_hit_player = true;

				// Play sound
				AudioPlayer::get().player_hit_swerve.play();
			}
		}

		// Check if player has picked up an upgrade
		if (UpgradeControl* upgrade = control->is<UpgradeControl>())
		{
			if (overlaps(upgrade->hitbox(), m_player.hitbox()) && !upgrade->is_picked_up())
			{
				// Change weapon of player and indicate that upgrade has been picked up
				m_weapon = upgrade->type();
				upgrade->pick_up();

				// Play upgrade sound
				AudioPlayer::get().upgrade.play();

				// Reset UpgradeControl
				upgrade->reset(state);
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

	// Ensure the player is always at least "constants::PADDING" away from the view's border
	m_player.clamp_position();

	// Shoot weapon if cooldown is over and space button is pressed
	if (
		m_shoot_cooldown <= 0.0f &&
		state.inputs.pressed_keys.contains(sf::Keyboard::Key::Space)
	)
	{
		if (m_weapon == constants::upgrades::Weapon::DEFAULT)
		{
			// Create Controller for new bullet at position of player
			state.controls.add<PlayerBulletControl>(sf::Vector2f{
				m_player.hitbox().getCenter().x - constants::player_bullet::BULLET_SIZE.x / 2.0f,
				m_player.hitbox().position.y - constants::player_bullet::BULLET_SIZE.y
			});
		}
		else if (m_weapon == constants::upgrades::Weapon::LASER)
		{
			// Create Controller for new bullet at position of player
			state.controls.add<LaserControl>(sf::Vector2f{
				m_player.hitbox().getCenter().x - constants::player_bullet::LASER_SIZE.x / 2.0f,
				m_player.hitbox().position.y - constants::player_bullet::LASER_SIZE.y
			});

			// Play laser sound effect
			AudioPlayer::get().laser.play();
		}
		else // m_weapon == Weapon::BOMB
		{
			// Create Controller for new bullet at position of player
			state.controls.add<BombControl>(sf::Vector2f{
				m_player.hitbox().getCenter().x - constants::player_bullet::BOMB_SIZE.x / 2.0f,
				m_player.hitbox().position.y - constants::player_bullet::BOMB_SIZE.y
			});
		}

		// Reset cooldown period
		m_shoot_cooldown = constants::player::SHOOT_COOLDOWN;

		// Reset weapon
		m_weapon = constants::upgrades::Weapon::DEFAULT;
	}
}

// Set weapon for next shot
void PlayerControl::set_weapon(constants::upgrades::Weapon weapon)
{
	m_weapon = weapon;
}


// Draw player
void PlayerControl::draw(LayerManager& layers)
{
	if (!m_hidden)
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

void PlayerControl::hide()
{
	m_hidden = true;
}
