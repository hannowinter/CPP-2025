#include "PlayerBulletControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

PlayerBulletControl::PlayerBulletControl(sf::Vector2f position) :
	m_bullet{ position }
{

}

void PlayerBulletControl::init(const ControlList& controls)
{

}

void PlayerBulletControl::update(const UpdateState& state)
{
	m_bullet.move_up(state.delta);
	if (!overlaps(constants::VIEW_RECT, m_bullet.hitbox())) // bullet is outside of view
		state.controls.remove(this);
}

void PlayerBulletControl::draw(LayerManager& layers)
{
	m_bullet_view.draw(layers.get(LayerID::ACTORS), m_bullet);
}

const PlayerBullet& PlayerBulletControl::get() const
{
	return m_bullet;
}

PlayerBullet& PlayerBulletControl::get()
{
	return m_bullet;
}