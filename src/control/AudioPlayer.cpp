#include "AudioPlayer.hpp"

#include "../view/AssetLoader.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include <iostream>

AudioPlayer::AudioPlayer() :
    m_alien_hit_buffer{},
    m_explosion_buffer{},
    m_game_over_buffer{},
    m_laser_buffer{},
    m_player_hit_bullet_buffer{},
    m_player_hit_swerve_buffer{},
    m_level_won_buffer{},
    m_upgrade_buffer{},
    alien_hit{ m_alien_hit_buffer },
    explosion{ m_explosion_buffer },
    game_over{ m_game_over_buffer },
    laser{ m_laser_buffer },
    player_hit_bullet{ m_player_hit_bullet_buffer },
    player_hit_swerve{ m_player_hit_swerve_buffer },
    level_won{ m_level_won_buffer },
    upgrade{ m_upgrade_buffer }
{
    bool success = true;
    success |= m_alien_hit_buffer.loadFromFile("assets/audio/alien_hit.wav");
    success |= m_explosion_buffer.loadFromFile("assets/audio/explosion.wav");
    success |= m_game_over_buffer.loadFromFile("assets/audio/game_over.wav");
    success |= m_laser_buffer.loadFromFile("assets/audio/laser.wav");
    success |= m_player_hit_bullet_buffer.loadFromFile("assets/audio/player_hit_bullet.wav");
    success |= m_player_hit_swerve_buffer.loadFromFile("assets/audio/player_hit_swerve.wav");
    success |= m_level_won_buffer.loadFromFile("assets/audio/level_won.wav");
    success |= m_upgrade_buffer.loadFromFile("assets/audio/upgrade.wav");

    if (!success)
        std::cerr << "Warning: One or multiple audio files failed to load!\n";
}

AudioPlayer& AudioPlayer::get()
{
    if (!inst)
        inst = std::make_unique<AudioPlayer>();
    return *inst;
}

std::unique_ptr<AudioPlayer> AudioPlayer::inst;