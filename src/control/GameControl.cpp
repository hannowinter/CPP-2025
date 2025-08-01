#include "GameControl.hpp"

#include "AlienControl.hpp"
#include "PlayerControl.hpp"
#include "AlienGridControl.hpp"
#include "AudioPlayer.hpp"
#include "SmokeControl.hpp"
#include "../model/Constants.hpp"

// Create GameController
GameControl::GameControl() :
    m_random{ static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count() 
    ) }
{

}

// Move to next level
void GameControl::increment_level()
{
    m_state.level++;
    m_state.lives = 5;
    m_state.over = false;
    m_state.level_won = false;


}

// Reset game to beginning
void GameControl::reset_game()
{
    m_state.level = 1;
    m_state.lives = 5;
    m_state.score = 0;
    m_state.over = false;
    m_state.level_won = false;
}

// Add child controllers
void GameControl::add_children(ControlList& controls)
{
    // Add PlayerController to list of controllers
    controls.add<PlayerControl>(sf::Vector2f{
       (constants::VIEW_WIDTH - constants::player::SIZE.x) / 2.0f,
       constants::VIEW_HEIGHT - constants::player::SIZE.y - constants::PADDING
        });

    // Add AlienGridController to list of controllers
    controls.add<AlienGridControl>();
}


// Initialize this controller
void GameControl::init(const ControlList& controls)
{
    // nothing to do here
}

// Execute relevant updates
void GameControl::update(const UpdateState& state)
{
    // Check if game is over (no lives left or aliens reached bottom)
    AlienGridControl* grid = state.controls.get<AlienGridControl>();
    if ((m_state.lives == 0 || grid->get_bottom() >= constants::VIEW_HEIGHT - 100.0f) && !m_gameover_shown)
    {
        // Play sound
        AudioPlayer::get().game_over.play();

        // Get PlayerController
        const PlayerControl* player_control = state.controls.get<PlayerControl>();

        // Create smoke at player position
        state.controls.remove(player_control);
        state.controls.add<SmokeControl>(player_control->get().hitbox().position);

        m_state.over = true;
        m_gameover_shown = true;
    }

    // Check if game should be restarted
    if (m_state.over && state.inputs.held_keys.contains(sf::Keyboard::Key::Space))
    {
        // Remove all controllers from list
        for (const auto& control : state.controls)
        {
            if (!control->is<GameControl>())
                state.controls.remove(control.get());
        }

        reset_game();
        add_children(state.controls);

        m_gameover_shown = false;
    }

    // Check if all alien have been shot and player can advance to next level
    if (state.controls.count<AlienControl>() == 0 && !m_victory_shown)
    {
        // Play sound
        AudioPlayer::get().level_won.play();

        m_state.level_won = true;
        m_victory_shown = true;
    }

    // Check if game should continue
    if (m_state.level_won && state.inputs.held_keys.contains(sf::Keyboard::Key::Space) && m_state.level < 10)
    {
        // Remove all controllers from list
        for (const auto& control : state.controls)
        {
            if (!control->is<GameControl>())
                state.controls.remove(control.get());
        }

        increment_level();
        add_children(state.controls);

        m_victory_shown = false;
    }
    else if (m_state.level_won && state.inputs.held_keys.contains(sf::Keyboard::Key::Space) && m_state.level == 10)
    {
        // Remove all controllers from list
        for (const auto& control : state.controls)
        {
            if (!control->is<GameControl>())
                state.controls.remove(control.get());
        }

        reset_game();
        add_children(state.controls);

        m_victory_shown = false;
    }
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