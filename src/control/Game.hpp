#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "../view/AssetLoader.hpp"
#include "../view/Layer.hpp"
#include "Controls.hpp"
#include "Inputs.hpp"

class Game 
{

public:
    Game();

    void start();

private:
    enum class PollResult_t
    {
        running,
        closed
    };

    PollResult_t poll_events();

    void update(float delta);

    void draw();

    sf::RenderWindow m_window;
    sf::View m_view;

    Inputs m_inputs;

    Layer m_actors_layer;

    ControlList m_control_list;
};

#endif