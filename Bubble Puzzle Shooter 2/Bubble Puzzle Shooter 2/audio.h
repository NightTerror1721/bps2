#pragma once

#include <SFML/Audio.hpp>

#include <string>
#include <map>

#include "utils.h"

class AudioController : singleton
{
private:
	std::map<std::string, std::pair<sf::SoundBuffer, sf::Sound>> _sounds;
	std::map<std::string, sf::Music*> _musics;
	sf::Music* _cmusic;

	unsigned int _size;

	static AudioController _instance;

	inline AudioController() : _sounds(), _musics(), _cmusic(nullptr) {}

public:
	~AudioController();

	void loadSound(const std::string& filename, const std::string& tag);

	void openMusic(const std::string& filename, const std::string& tag);
	void freeMusic(const std::string& tag);

	void playSound(const std::string& tag);
	sf::Sound* getSound(const std::string& tag);

	void playMusic(const std::string& tag = "");
	void stopMusic();
	void pauseMusic();

	friend AudioController& GetAudioController();
};

__forceinline AudioController& GetAudioController() { return AudioController::_instance; }

#define AUDIO_CONTROLLER GetAudioController()
#define audio_loadSound(filename, tag) AUDIO_CONTROLLER.loadSound((filename), (tag))
#define audio_openMusic(filename, tag) AUDIO_CONTROLLER.openMusic((filename), (tag))
#define audio_freeMusic(tag) AUDIO_CONTROLLER.freeMusic((tag))
#define audio_playSound(tag) AUDIO_CONTROLLER.playSound((tag))
#define audio_getSound(tag) AUDIO_CONTROLLER.getSound((tag))
#define audio_playMusic(tag) AUDIO_CONTROLLER.playMusic((tag))
#define audio_replayMusic() AUDIO_CONTROLLER.playMusic()
#define audio_stopMusic() AUDIO_CONTROLLER.stopMusic()
#define audio_pauseMusic() AUDIO_CONTROLLER.pauseMusic()


