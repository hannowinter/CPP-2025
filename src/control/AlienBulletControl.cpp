#include "AlienBulletControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

AlienBulletControl::AlienBulletControl(sf::Vector2f position) :
	m_bullet{ position }
{

}

void AlienBulletControl::init(const ControlList& controls)
{

}

void AlienBulletControl::update(const UpdateState& state)
{
	m_bullet.move_down(state.delta);
	if (!overlaps(constants::VIEW_RECT, m_bullet.hitbox())) // bullet is outside of view
		state.controls.remove(this);
	m_bullet_view.update(state.delta);
}

void AlienBulletControl::draw(LayerManager& layers)
{
	m_bullet_view.draw(layers.get(LayerID::ACTORS), m_bullet);
}

const AlienBullet& AlienBulletControl::get() const
{
	return m_bullet;
}

AlienBullet& AlienBulletControl::get()
{
	return m_bullet;
}