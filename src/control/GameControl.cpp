#include "GameControl.hpp"

#include "PlayerControl.hpp"
#include "AlienGridControl.hpp"
#include "../model/Constants.hpp"

GameControl::GameControl(ControlList& controls) :
    m_random{ static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count() 
    ) }
{
    controls.add<PlayerControl>(sf::Vector2f{
       (constants::VIEW_WIDTH - constants::player::SIZE.x) / 2.0f,
       constants::VIEW_HEIGHT - constants::player::SIZE.y - constants::PADDING
        });
    controls.add<AlienGridControl>(controls);
}

void GameControl::init(const ControlList& controls)
{

}

void GameControl::update(const UpdateState& state)
{

}

void GameControl::draw(Layer& layer)
{

}

std::mt19937& GameControl::random()
{
    return m_random;
}