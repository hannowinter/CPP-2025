#include "BulletControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

BulletControl::BulletControl(sf::Vector2f position) :
	m_bullet{ position }
{

}

void BulletControl::update(const UpdateState& state)
{
	m_bullet.move_up(state.delta);
	if (!overlaps(constants::VIEW_RECT, m_bullet.hitbox()))
		state.actors.despawn(this);
}

void BulletControl::draw(Layer& layer)
{
	m_bullet_view.draw(layer, m_bullet);
}