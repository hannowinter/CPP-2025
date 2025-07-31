#include "Engine.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "GameControl.hpp"
#include "../model/Constants.hpp"

// Create new game instance with defined parameters and initialize GameController
Engine::Engine() :
    m_window{
        sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, 
        "Space Invaders"
    },
    m_view{ constants::VIEW_RECT },
    m_layer_manager{ m_window }
{
    m_window.setFramerateLimit(constants::FRAME_RATE);
}

// Start game
void Engine::start() 
{
    // Game clock
    sf::Clock clock;

    // Add the main `GameControl`
    const GameControl& game_control = m_control_list.add<GameControl>();

    m_control_list.execute_requests();

    // While game is running, read new state und update view
    while (m_window.isOpen())
    {
        sf::Time elapsed_time = clock.restart();
        if (game_control.state().over)
            elapsed_time = sf::Time::Zero;

        PollResult_t poll_result = poll_events();
        if (poll_result == PollResult_t::closed)
            break;

        update(elapsed_time.asSeconds());
        draw();
    }

    // Close window if game has been closed
    m_window.close();
}

// Poll inputs and other events (window closed, etc.)
Engine::PollResult_t Engine::poll_events()
{
    m_inputs.update();

    while (std::optional<sf::Event> event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            return PollResult_t::closed;

        m_inputs.event(*event);
    }
    return PollResult_t::running;
}

// Update all controllers and set current view
void Engine::update(float delta)
{
    m_control_list.update(delta, m_inputs);
    m_control_list.execute_requests();

    m_layer_manager.set_view(m_view);
}

// Clear window and layers and draw new state
void Engine::draw()
{
    m_window.clear();

    m_layer_manager.clear();

    m_control_list.draw(m_layer_manager);
    
    m_layer_manager.draw();

    m_window.display();
}
