#include "PlayerProjectileControl.hpp"

#include "../Util.hpp"
#include "../model/Constants.hpp"
#include "../model/PlayerLaser.hpp"
#include "../view/LaserView.hpp"

// Create PlayerProjectileControl for PlayerProjectile at position
PlayerProjectileControl::PlayerProjectileControl(std::unique_ptr<PlayerProjectile> projectile,
                                                 std::unique_ptr<PlayerProjectileView> view) :
    m_projectile{ std::move(projectile) },
    m_projectile_view{ std::move(view) }
{

}

// Initialize this controller
void PlayerProjectileControl::init(const ControlList& controls)
{
    // nothing to do here
}

// Execute relevant updates
void PlayerProjectileControl::update(const UpdateState& state)
{
    // Make projectile move up depending on elapsed time
    m_projectile->move_up(state.delta);

    // Delete projectile if it has left the scene
    if (!overlaps(constants::VIEW_RECT, m_projectile->hitbox())) // projectile is outside of view
        state.controls.remove(this);
}

// Draw PlayerProjectile
void PlayerProjectileControl::draw(LayerManager& layers)
{
    m_projectile_view->draw(layers.get(LayerID::ACTORS), *m_projectile);
}

// Get reference to Bullet
const PlayerProjectile& PlayerProjectileControl::get() const
{
    return *m_projectile;
}

// Get reference to Bullet
PlayerProjectile& PlayerProjectileControl::get()
{
    return *m_projectile;
}