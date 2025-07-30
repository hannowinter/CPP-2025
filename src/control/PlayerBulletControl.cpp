#include "PlayerBulletControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

// Create BulletController for Bullet at position
PlayerBulletControl::PlayerBulletControl(sf::Vector2f position) :
	m_bullet{ position }
{

}

// Initialize this controller
void PlayerBulletControl::init(const ControlList& controls)
{
	// nothing to do here
}

// Execute relevant updates
void PlayerBulletControl::update(const UpdateState& state)
{
	// Make bullet move up depending on elapsed time
	m_bullet.move_up(state.delta);

	// Delete bullet if it has left the scene
	if (!overlaps(constants::VIEW_RECT, m_bullet.hitbox())) // bullet is outside of view
		state.controls.remove(this);
}

// Draw Bullet
void PlayerBulletControl::draw(LayerManager& layers)
{
	m_bullet_view.draw(layers.get(LayerID::ACTORS), m_bullet);
}

// Get reference to Bullet
const PlayerBullet& PlayerBulletControl::get() const
{
	return m_bullet;
}

// Get reference to Bullet
PlayerBullet& PlayerBulletControl::get()
{
	return m_bullet;
}