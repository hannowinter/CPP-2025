#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "../view/AssetLoader.hpp"
#include "../view/LayerManager.hpp"
#include "Controls.hpp"
#include "Inputs.hpp"

// Class modelling the Game
class Game 
{

public:
    // Create new game instance
    Game();

    // Start game
    void start();

private:
    // Possible game states
    enum class PollResult_t
    {
        running,
        closed
    };

    // Upate game state and read inputs
    PollResult_t poll_events();

    // Update game
    void update(float delta);

    // Draw game
    void draw();

    // Game window and main view
    sf::RenderWindow m_window;
    sf::View m_view;

    // Active inputs
    Inputs m_inputs;

    // Manager for all game layers
    LayerManager m_layer_manager;

    // List of all active controllers
    ControlList m_control_list;
};

#endif