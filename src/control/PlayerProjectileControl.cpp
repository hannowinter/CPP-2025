#include "PlayerProjectileControl.hpp"

#include "../Util.hpp"
#include "../model/Constants.hpp"

// Initializes this control.
void PlayerProjectileControl::init(const ControlList& controls)
{
    // nothing to do here
}

// Executes relevant updates.
void PlayerProjectileControl::update(const UpdateState& state)
{
    // Make projectile move up depending on elapsed time
    m_projectile->move_up(state.delta);

    // Delete projectile if it has left the scene
    if (!overlaps(constants::VIEW_RECT, m_projectile->hitbox())) // projectile is outside of view
        state.controls.request_remove(this);
}

// Draws PlayerProjectile.
void PlayerProjectileControl::draw(LayerManager& layers)
{
    m_projectile_view->draw(layers.get(LayerID::ACTORS), *m_projectile);
}

// Gets reference to model object.
const PlayerProjectile& PlayerProjectileControl::get() const
{
    return *m_projectile;
}

PlayerProjectile& PlayerProjectileControl::get()
{
    return *m_projectile;
}



// =======================================================



// Creates control at position.
PlayerBulletControl::PlayerBulletControl(sf::Vector2f position) :
    PlayerProjectileControl{ PlayerBullet{ position }, PlayerBulletView{} }
{

}



// =======================================================



// Creates control at position.
LaserControl::LaserControl(sf::Vector2f position) :
    PlayerProjectileControl{ PlayerLaser{ position }, LaserView{} }
{

}



// ======================================================



// Creates control at position.
BombControl::BombControl(sf::Vector2f position) :
    PlayerProjectileControl{ PlayerBomb{ position }, BombView{ } },
    m_lifetime{ constants::upgrades::EXPLOSION_TIME }
{

}

// Executes all relevant updates.
void BombControl::update(const UpdateState& state)
{
    // Make bomb move up depending on elapsed time if it has not yet exploded
    if (!m_has_exploded)
        m_projectile->move_up(state.delta);

    // Delete bomb if it has left the scene
    if (!overlaps(constants::VIEW_RECT, m_projectile->hitbox())) // projectile is outside of view
        state.controls.request_remove(this);

    // Decrement lifetime if bomb has exploded
    if (m_has_exploded)
        m_lifetime -= state.delta;

    // Delete explosion if timer is over
    if (m_lifetime <= 0.0f)
        state.controls.request_remove(this);
}

// Makes bomb explode.
void BombControl::explode()
{
    static_cast<BombView*>(m_projectile_view.get())->explode();
    static_cast<PlayerBomb*>(m_projectile.get())->explode();

    m_has_exploded = true;
}

// Checks if bomb has exploded.
bool BombControl::has_exploded() const
{
    return m_has_exploded;
}
