#include "AlienBulletControl.hpp"
#include "../model/Constants.hpp"

// Create BulletController with Bullet at position
AlienBulletControl::AlienBulletControl(sf::Vector2f position) :
	m_bullet{ position }
{

}

// Initialize this controller
void AlienBulletControl::init(const ControlList& controls)
{
	// nothing to do here
}

// Execute all relevant updates
void AlienBulletControl::update(const UpdateState& state)
{
	// Make bullet move down depending on elapsed time
	m_bullet.move_down(state.delta);

	// Delete the bullet if it has left the scene
	if (m_bullet.outside_view())
		state.controls.request_remove(this);

	// Update BulletView (the AlienBullet is animated)
	m_bullet_view.update(state.delta);
}

// Draw Bullet
void AlienBulletControl::draw(LayerManager& layers)
{
	m_bullet_view.draw(layers.get(LayerID::ACTORS), m_bullet);
}

// Get reference to Bullet
const AlienBullet& AlienBulletControl::get() const
{
	return m_bullet;
}

// Get reference to Bullet
AlienBullet& AlienBulletControl::get()
{
	return m_bullet;
}