#include "BombControl.hpp"

#include "../model/Constants.hpp"
#include "../model/PlayerBomb.hpp"
#include "../view/BombView.hpp"
#include "../Util.hpp"

// Create BombControl at position
BombControl::BombControl(sf::Vector2f position) :
    PlayerProjectileControl(std::make_unique<PlayerBomb>(position), std::make_unique<BombView>()),
    m_lifetime { constants::upgrades::EXPLOSION_TIME }
{

}

// Execute all relevant updates
void BombControl::update(const UpdateState& state)
{
    // Make bomb move up depending on elapsed time if it has not yet exploded
    if (!m_has_exploded)
        m_projectile->move_up(state.delta);

    // Delete bomb if it has left the scene
    if (!overlaps(constants::VIEW_RECT, m_projectile->hitbox())) // projectile is outside of view
        state.controls.remove(this);

    // Decrement lifetime if bomb has exploded
    if (m_has_exploded)
        m_lifetime -= state.delta;

    // Delete explosion if timer is over
    if (m_lifetime <= 0.0f)
        state.controls.remove(this);
}


// Make bomb explode
void BombControl::explode()
{
    static_cast<BombView*>(m_projectile_view.get())->explode();
    static_cast<PlayerBomb*>(m_projectile.get())->explode();

    m_has_exploded = true;
}

// Check if bomb has exploded
bool BombControl::has_exploded()
{
    return m_has_exploded;
}
