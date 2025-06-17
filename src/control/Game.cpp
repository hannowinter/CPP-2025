#include "Game.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "../model/Constants.hpp"

Game::Game() : window(sf::VideoMode({ constants::VIEW_WIDTH, constants::VIEW_HEIGHT }), "Space Invaders"),
    view(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(constants::VIEW_WIDTH, constants::VIEW_HEIGHT))),
    game_layer(window),
    hello_world_control(game_layer) {
    // limit frame rate
    window.setFramerateLimit(constants::FRAME_RATE);

    // set the view (visible area) for our game
    game_layer.set_view(view);
}

void Game::start() {
    // The clock is needed to control the speed of movement
    sf::Clock clock;

    while (window.isOpen()) {
        // Restart the clock and save the elapsed time into elapsed_time
        sf::Time elapsed_time = clock.restart();
 
        // handle input, check if window is still open
        if (!input()) {
            // update the scene according to the passed time
            update(elapsed_time.asSeconds());
            // draw the scene
            draw();
        }
    }
}

// returns true, if the window has been closed
bool Game::input() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            // quit
            window.close();
            return true;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
            case sf::Keyboard::Key::W:
                hello_world_control.move(sf::Vector2(0.0f, -5.0f));
                break;
            case sf::Keyboard::Key::A:
                hello_world_control.move(sf::Vector2(-5.0f, 0.0f));
                break;
            case sf::Keyboard::Key::S:
                hello_world_control.move(sf::Vector2(0.0f, 5.0f));
                break;
            case sf::Keyboard::Key::D:
                hello_world_control.move(sf::Vector2(5.0f, 0.0f));
                break;
            }
        }
    }
    return false;
}

void Game::update(float time_passed) {
    // TODO: update the game objects with the current time stamp
    game_layer.set_view(view);
}

void Game::draw() {
    window.clear();

    game_layer.clear();

    hello_world_control.draw();
    
    // TODO: add game elements to layer
    game_layer.draw();

    window.display();
}
