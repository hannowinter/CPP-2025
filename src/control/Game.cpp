#include "Game.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "../model/Constants.hpp"

Game::Game() :
    m_window{
        sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, 
        "Space Invaders"
    },
    m_view{ sf::FloatRect{ { 0.0f, 0.0f }, { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } } },
    m_actors_layer{ m_window }
{
    m_window.setFramerateLimit(constants::FRAME_RATE);
    m_actors.spawn<PlayerControl>({
        (constants::VIEW_WIDTH - constants::PLAYER_SIZE.x) / 2.0f,
        constants::VIEW_HEIGHT - constants::PLAYER_SIZE.y - constants::PADDING
    });
}

void Game::start() 
{
    sf::Clock clock;

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
    m_actors.update(delta, m_inputs);

    m_actors_layer.set_view(m_view);
}

void Game::draw() 
{
    m_window.clear();

    m_actors_layer.clear();

    m_actors.draw(m_actors_layer);
    
    m_actors_layer.draw();

    m_window.display();
}
