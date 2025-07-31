#include "Game.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "GameControl.hpp"
#include "../model/Constants.hpp"

// Create new game instance with defined parameters and initialize GameController
Game::Game() :
    m_window{
        sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, 
        "Space Invaders"
    },
    m_view{ constants::VIEW_RECT },
    m_layer_manager{ m_window }
{
    m_window.setFramerateLimit(constants::FRAME_RATE);
    m_control_list.add<GameControl>();
}

// Start game
void Game::start() 
{
    // Game clock
    sf::Clock clock;

    // Initialize controllers
    m_control_list.init();

    // Get GameControl
    const GameControl* game_control = m_control_list.get<GameControl>();

    // While game is running, read new state und update view
    while (m_window.isOpen())
    {
        sf::Time elapsed_time = clock.restart();
        if (game_control->state().over)
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

// Read inputs and check game state
Game::PollResult_t Game::poll_events()
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
void Game::update(float delta) 
{
    // Get GameController
    GameControl* game_control = m_control_list.get<GameControl>();

    // Check if game should be restarted
    if (game_control->state().over && m_inputs.held_keys.contains(sf::Keyboard::Key::Space))
    {
        // Remove all controllers from list
        for (const auto& control : m_control_list)
        {
            if (!control->is<GameControl>())
                m_control_list.remove(control.get());
        }

        game_control->increment_level();
        game_control->spawn_children(m_control_list);
        m_control_list.init();
    }

    m_control_list.update(delta, m_inputs);

    m_layer_manager.set_view(m_view);
}

// Clear window and layers and draw new state
void Game::draw() 
{
    m_window.clear();

    m_layer_manager.clear();

    // TODO:
    // don't just pass the actors layer
    m_control_list.draw(m_layer_manager);
    
    m_layer_manager.draw();

    m_window.display();
}
