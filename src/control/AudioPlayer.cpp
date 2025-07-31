#include "AudioPlayer.hpp"

#include "../view/AssetLoader.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

AudioPlayer::AudioPlayer() :
    alien_hit_buffer(),
    explosion_buffer(),
    game_over_buffer(),
    laser_buffer(),
    player_hit_bullet_buffer(),
    player_hit_swerve_buffer(),
    alien_hit(alien_hit_buffer),
    explosion(explosion_buffer),
    game_over(game_over_buffer),
    laser(laser_buffer),
    player_hit_bullet(player_hit_bullet_buffer),
    player_hit_swerve(player_hit_swerve_buffer)
{
    alien_hit_buffer.loadFromFile("assets/audio/alien_hit.wav");
    explosion_buffer.loadFromFile("assets/audio/explosion.wav");
    game_over_buffer.loadFromFile("assets/audio/game_over.wav");
    laser_buffer.loadFromFile("assets/audio/laser.wav");
    player_hit_bullet_buffer.loadFromFile("assets/audio/player_hit_bullet.wav");
    player_hit_swerve_buffer.loadFromFile("assets/audio/player_hit_swerve.wav");

    alien_hit.setBuffer(alien_hit_buffer);
    explosion.setBuffer(explosion_buffer);
    game_over.setBuffer(game_over_buffer);
    laser.setBuffer(laser_buffer);
    player_hit_bullet.setBuffer(player_hit_bullet_buffer);
    player_hit_swerve.setBuffer(player_hit_swerve_buffer);
}

AudioPlayer& AudioPlayer::get()
{
    if (!inst)
        inst = std::make_unique<AudioPlayer>();
    return *inst;
}

std::unique_ptr<AudioPlayer> AudioPlayer::inst;