#pragma once

#include <memory>
#include <SFML/Audio.hpp>

#include "manager.h"

class AudioManager : public Manager<sf::Sound>
{
public:
	AudioManager(AudioManager* const& parent = nullptr);

	void load(const std::string& filename, const std::string& name);
};

using music_t = std::unique_ptr<sf::Music>;
namespace Music
{
	music_t open(const std::string& filename);
	void free(music_t& music);

	void play(music_t& music);
	void stop(music_t& music);
	void pause(music_t& music);
}

namespace
{
	std::string build_path(const std::string& filename);
}
