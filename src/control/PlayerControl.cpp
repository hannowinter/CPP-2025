#include "PlayerControl.hpp"
#include "../model/Constants.hpp"
#include "BulletControl.hpp"

PlayerControl::PlayerControl(sf::Vector2f position) :
	m_player{ position }
{

}

void PlayerControl::update(const UpdateState& state) 
{
	if (state.inputs.held_keys.contains(sf::Keyboard::Key::Left))
	{
		m_player.move(Player::LEFT, state.delta);
	}
	if (state.inputs.held_keys.contains(sf::Keyboard::Key::Right))
	{
		m_player.move(Player::RIGHT, state.delta);
	}
	if (state.inputs.pressed_keys.contains(sf::Keyboard::Key::Space))
	{
		state.controls.add<BulletControl>(sf::Vector2f{
			m_player.hitbox().getCenter().x - constants::BULLET_SIZE.x / 2.0f,
			m_player.hitbox().position.y - constants::BULLET_SIZE.y
		});
	}
}

void PlayerControl::draw(Layer& layer) 
{
	m_player_view.draw(layer, m_player);
}