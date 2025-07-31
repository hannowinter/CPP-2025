#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "../view/AssetLoader.hpp"
#include "../view/LayerManager.hpp"
#include "Controls.hpp"
#include "Inputs.hpp"

// Class modelling the Game's engine
// Responsible for managing the game window, layers, inputs and holding all controls
class Engine 
{

public:
    // Create new `Engine` instance
    Engine();

    // Start game
    void start();

private:
    // Possible event poll results
    enum class PollResult_t
    {
        running,
        closed
    };

    // Poll inputs and other events (window closed, etc.)
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