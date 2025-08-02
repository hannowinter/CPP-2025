#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP
#include <memory>

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

// Struct to hold and play all audio files (Singleton)
struct AudioPlayer {
private:
    sf::SoundBuffer alien_hit_buffer;
    sf::SoundBuffer explosion_buffer;
    sf::SoundBuffer game_over_buffer;
    sf::SoundBuffer laser_buffer;
    sf::SoundBuffer player_hit_bullet_buffer;
    sf::SoundBuffer player_hit_swerve_buffer;
    sf::SoundBuffer level_won_buffer;
    sf::SoundBuffer upgrade_buffer;

    static std::unique_ptr<AudioPlayer> inst;

public:
    sf::Sound alien_hit;
    sf::Sound explosion;
    sf::Sound game_over;
    sf::Sound laser;
    sf::Sound player_hit_bullet;
    sf::Sound player_hit_swerve;
    sf::Sound level_won;
    sf::Sound upgrade;

    AudioPlayer();

    static AudioPlayer& get();


};

#endif
