#include "SmokeControl.hpp"

#include "../model/Constants.hpp"

// Creates control at position.
SmokeControl::SmokeControl(sf::Vector2f position) :
    m_smoke_view{},
    m_position{ position },
    m_lifetime{ constants::alien::EFFECT_DURATION }
{

}

// Initializes SmokeControl.
void SmokeControl::init(const ControlList& controls)
{
    // nothing to do here
}

// Executes all relevant updates.
void SmokeControl::update(const UpdateState& state)
{
    // Decrement remaining lifetime
    m_lifetime -= state.delta;

    // Delete smoke if lifetime is over
    if (m_lifetime <= 0.0f)
        state.controls.request_remove(this);
}

// Draws Smoke to screen.
void SmokeControl::draw(LayerManager& layers)
{
    m_smoke_view.draw(layers.get(LayerID::EFFECTS), m_position);
}