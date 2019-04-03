#include "audio.h"
#include "py.h"

#define CHECK_SIZE if(this->_size >= 255) { return; }

AudioController AudioController::_instance;

AudioController::~AudioController()
{
	stopMusic();
	_sounds.clear();
	_musics.clear();
}

void AudioController::loadSound(const std::string& filename, const std::string& tag)
{
	CHECK_SIZE
	if (_sounds.find(tag) != _sounds.cend())
		return;

	_sounds[tag] = { sf::SoundBuffer(), sf::Sound() };
	std::pair<sf::SoundBuffer, sf::Sound>& ps = _sounds[tag];

	if (!ps.first.loadFromFile(filename))
	{
		auto it = _sounds.find(tag);
		if (it != _sounds.end())
			_sounds.erase(it);
		return;
	}
	ps.second.setBuffer(ps.first);
	_size++;
}

void AudioController::openMusic(const std::string& filename, const std::string& tag)
{
	CHECK_SIZE
	if (_musics.find(tag) != _musics.cend())
		return;

	auto music = new sf::Music();
	if (!music->openFromFile(filename))
		return;

	_musics[tag] = music;
	_size++;
}

void AudioController::freeMusic(const std::string& tag)
{
	auto it = _musics.find(tag);
	if (it == _musics.end())
		return;

	auto music = (*it).second;
	_musics.erase(it);
	if (music == _cmusic)
		stopMusic();

	delete music;
}

void AudioController::playSound(const std::string& tag)
{
	auto it = _sounds.find(tag);
	if (it != _sounds.cend())
	{
		auto& sound = (*it).second.second;
		if (sound.getStatus() != sf::Sound::Playing)
			sound.play();
	}
}

sf::Sound* AudioController::getSound(const std::string& tag)
{
	auto it = _sounds.find(tag);
	if (it != _sounds.cend())
		return &(*it).second.second;
	return nullptr;
}

void AudioController::playMusic(const std::string& tag)
{
	auto it = _musics.find(tag);
	if (it != _musics.cend())
	{
		auto music = (*it).second;
		if (_cmusic)
		{
			if (_cmusic == music)
				if (_cmusic->getStatus() == sf::Music::Playing)
					return;
			else
			{
				stopMusic();
				_cmusic = music;
			}
		}
		music->play();
	}
}

void AudioController::stopMusic()
{
	if (_cmusic)
	{
		if (_cmusic->getStatus() != sf::Music::Stopped)
			_cmusic->stop();
		_cmusic = nullptr;
	}
}

void AudioController::pauseMusic()
{
	if (_cmusic)
	{
		if (_cmusic->getStatus() != sf::Music::Paused)
			_cmusic->pause();
	}
}



PYBIND11_EMBEDDED_MODULE(__audio, m) {
	m.def("loadSound", [](const std::string& filename, const std::string& tag) {
		audio_loadSound(filename, tag);
	});

	m.def("openMusic", [](const std::string& filename, const std::string& tag) {
		audio_openMusic(filename, tag);
	});

	m.def("playSound", [](const std::string& tag) {
		audio_playSound(tag);
	});

	m.def("playMusic", [](const std::string& tag) {
		audio_playMusic(tag);
	});

	m.def("stopMusic", []() {
		audio_stopMusic();
	});
}

