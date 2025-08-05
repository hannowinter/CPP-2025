#include "AudioPlayer.hpp"

#include "../view/AssetLoader.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include <iostream>

AudioPlayer::AudioPlayer() :
    alien_hit_buffer(),
    explosion_buffer(),
    game_over_buffer(),
    laser_buffer(),
    player_hit_bullet_buffer(),
    player_hit_swerve_buffer(),
    level_won_buffer(),
    upgrade_buffer(),
    alien_hit(alien_hit_buffer),
    explosion(explosion_buffer),
    game_over(game_over_buffer),
    laser(laser_buffer),
    player_hit_bullet(player_hit_bullet_buffer),
    player_hit_swerve(player_hit_swerve_buffer),
    level_won(level_won_buffer),
    upgrade(upgrade_buffer)
{
    bool success = true;
    success |= alien_hit_buffer.loadFromFile("assets/audio/alien_hit.wav");
    success |= explosion_buffer.loadFromFile("assets/audio/explosion.wav");
    success |= game_over_buffer.loadFromFile("assets/audio/game_over.wav");
    success |= laser_buffer.loadFromFile("assets/audio/laser.wav");
    success |= player_hit_bullet_buffer.loadFromFile("assets/audio/player_hit_bullet.wav");
    success |= player_hit_swerve_buffer.loadFromFile("assets/audio/player_hit_swerve.wav");
    success |= level_won_buffer.loadFromFile("assets/audio/level_won.wav");
    success |= upgrade_buffer.loadFromFile("assets/audio/upgrade.wav");

    if (!success)
        std::cerr << "Warning: One or multiple audio files failed to load!\n";

    alien_hit.setBuffer(alien_hit_buffer);
    explosion.setBuffer(explosion_buffer);
    game_over.setBuffer(game_over_buffer);
    laser.setBuffer(laser_buffer);
    player_hit_bullet.setBuffer(player_hit_bullet_buffer);
    player_hit_swerve.setBuffer(player_hit_swerve_buffer);
    level_won.setBuffer(level_won_buffer);
    upgrade.setBuffer(upgrade_buffer);
}

AudioPlayer& AudioPlayer::get()
{
    if (!inst)
        inst = std::make_unique<AudioPlayer>();
    return *inst;
}

std::unique_ptr<AudioPlayer> AudioPlayer::inst;