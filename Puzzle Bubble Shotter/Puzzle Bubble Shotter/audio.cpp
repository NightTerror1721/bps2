#include "audio.h"
#include "paths.h"

AudioManager::AudioManager(AudioManager* const& parent) :
	Manager(parent)
{}

void AudioManager::load(const std::string& filename, const std::string& name)
{
	sf::SoundBuffer sb;
	if (!sb.loadFromFile(Path::Sounds + filename))
		return;

	auto sound = create(name);
	sound->setBuffer(std::move(sb));
}

music_t Music::open(const std::string& filename)
{
	music_t music(new sf::Music());
	if (!music->openFromFile(Path::Musics + filename))
	{
		music.release();
		return nullptr;
	}
	return std::move(music);
}

void Music::free(music_t& music)
{
	if (music)
	{
		stop(music);
		music.release();
	}
}

void Music::play(music_t& music)
{
	if (music)
		music->play();
}

void Music::stop(music_t& music)
{
	if (music)
		music->stop();
}

void Music::pause(music_t& music)
{
	if (music)
		music->pause();
}
