#include "GameControl.hpp"

#include "PlayerControl.hpp"
#include "AlienGridControl.hpp"
#include "../model/Constants.hpp"

// Create GameController
GameControl::GameControl(ControlList& controls) :
    m_random{ static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count() 
    ) }
{
    // Add PlayerController to list of controllers
    controls.add<PlayerControl>(sf::Vector2f{
       (constants::VIEW_WIDTH - constants::player::SIZE.x) / 2.0f,
       constants::VIEW_HEIGHT - constants::player::SIZE.y - constants::PADDING
        });

    // Add AlienGridController to list of controllers
    controls.add<AlienGridControl>(controls);
}

// Initialize this controller
void GameControl::init(const ControlList& controls)
{
    // nothing to do here
}

// Execute relevant updates
void GameControl::update(const UpdateState& state)
{
    // nothing to do here
}

// Draw HUD
void GameControl::draw(LayerManager& layers)
{
    m_hud.draw(layers.get(LayerID::HUD), m_state);
}

// Get pseudo-random number generator
std::mt19937& GameControl::random()
{
    return m_random;
}

// Get current state
GameState& GameControl::state()
{
    return m_state;
}

// Get current state
const GameState& GameControl::state() const
{
    return m_state;
}