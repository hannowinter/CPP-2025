#include "PlayerControl.hpp"
#include "../model/Constants.hpp"
#include "PlayerBulletControl.hpp"

PlayerControl::PlayerControl(sf::Vector2f position) :
	m_player{ position },
	m_player_view{},
	m_shoot_cooldown{ 0.0f }
{

}

void PlayerControl::init(const ControlList& controls)
{

}

void PlayerControl::update(const UpdateState& state) 
{
	m_shoot_cooldown -= state.delta;

	if (state.inputs.held_keys.contains(sf::Keyboard::Key::Left))
	{
		m_player.move(Player::LEFT, state.delta);
	}
	if (state.inputs.held_keys.contains(sf::Keyboard::Key::Right))
	{
		m_player.move(Player::RIGHT, state.delta);
	}
	if (
		m_shoot_cooldown <= 0.0f &&
		state.inputs.pressed_keys.contains(sf::Keyboard::Key::Space)
	)
	{
		state.controls.add<PlayerBulletControl>(sf::Vector2f{
			m_player.hitbox().getCenter().x - constants::player_bullet::SIZE.x / 2.0f,
			m_player.hitbox().position.y - constants::player_bullet::SIZE.y
		});
		m_shoot_cooldown = constants::player::SHOOT_COOLDOWN;
	}
}

void PlayerControl::draw(Layer& layer) 
{
	m_player_view.draw(layer, m_player);
}

const Player& PlayerControl::get() const
{
	return m_player;
}

Player& PlayerControl::get()
{
	return m_player;
}