#include "SmokeControl.hpp"

#include "../model/Constants.hpp"

// Create SmokeController
SmokeControl::SmokeControl(sf::Vector2f position) :
    m_smoke_view {},
    m_position { position },
    m_lifetime { constants::alien::SMOKE_LIFETIME }
{

}

// Initialize SmokeController
void SmokeControl::init(const ControlList& controls)
{
    // nothing to do here
}

// Execute all relevant updates
void SmokeControl::update(const UpdateState& state)
{
    // Decrement remaining lifetime
    m_lifetime -= state.delta;

    // Delete smoke if lifetime is over
    if (m_lifetime <= 0.0f)
        state.controls.remove(this);
}

// Draw Smoke to screen
void SmokeControl::draw(LayerManager& layers)
{
    m_smoke_view.draw(layers.get(LayerID::EFFECTS), m_position);
}