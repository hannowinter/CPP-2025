#include "Game.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "GameControl.hpp"
#include "../model/Constants.hpp"

Game::Game() :
    m_window{
        sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, 
        "Space Invaders"
    },
    m_view{ constants::VIEW_RECT },
    m_layer_manager{ m_window }
{
    m_window.setFramerateLimit(constants::FRAME_RATE);
    m_control_list.add<GameControl, ControlList&>(m_control_list);
}

void Game::start() 
{
    sf::Clock clock;

    m_control_list.init();

    while (m_window.isOpen())
    {
        sf::Time elapsed_time = clock.restart();
 
        PollResult_t poll_result = poll_events();
        if (poll_result == PollResult_t::closed)
            break;

        update(elapsed_time.asSeconds());
        draw();
    }

    m_window.close();
}

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

void Game::update(float delta) 
{
    m_control_list.update(delta, m_inputs);

    m_layer_manager.set_view(m_view);
}

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
