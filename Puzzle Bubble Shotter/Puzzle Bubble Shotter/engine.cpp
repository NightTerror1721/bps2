#include "engine.h"

#include <SFML/OpenGL.hpp>
#include <algorithm>

template<class _Ty>
void clear_ptr_vector(std::vector<_Ty*>& vec)
{
	auto it = vec.begin();
	while (it != vec.end())
	{
		delete *it;
		it = vec.erase(it);
	}
}

GameController::GameController(const std::string& name) :
	InputListener(),
	_close(false),
	_window(),
	_py(),
	_deltaClock(),
	_phAccumulator(),
	_phUps(sf::seconds(1.f / 60.f)),
	_name(name),
	_vmode(1280, 720),
	_wstyle(wstyle::Default),
	_objs(),
	_events()
{
	_events.registerListener(this);
}

GameController::~GameController()
{
	clear_ptr_vector(_objs);
}

void GameController::start()
{
	init();
	loop();
}

void GameController::close()
{
	if (!_close)
	{
		_close = true;
		_window.close();
	}
}

void GameController::setVideoMode(sf::VideoMode mode, bool apply)
{
	_vmode = mode;
	if (apply)
		resetWindow();
}

void GameController::setStyle(u32 style, bool apply)
{
	_wstyle = style;
	if (apply)
		resetWindow();
}

void GameController::resetWindow()
{
	if (_close)
		return;

	if (_window.isOpen())
		close();
	_window.create(_vmode, _name.c_str(), _wstyle);
	_window.setVerticalSyncEnabled(true);
	_window.setActive(true);
}

bool GameController::isFullscreen() const { return _wstyle & wstyle::Fullscreen; }

sf::RenderWindow* GameController::getWindow() { return &_window; }
const sf::RenderWindow* GameController::getWindow() const { return &_window; }

void GameController::destroyGameObject(GameObject* const& obj_ptr)
{
	auto it = std::find(_objs.begin(), _objs.end(), obj_ptr);
	if (it != _objs.end())
		delete *it;
}


void GameController::loop()
{
	while (!_close)
	{
		processEvents();

		sf::Time delta = _deltaClock.getElapsedTime();
		_phAccumulator += delta;

		while (_phAccumulator > _phUps)
		{
			_phAccumulator -= _phUps;
			_deltaClock.restart();
			update(delta.asSeconds());
			delta = _deltaClock.getElapsedTime();
		}

		render();
	}
}

void GameController::init()
{
	resetWindow();
}

void GameController::update(delta_t delta)
{
	if (!_close)
		for (GameObject* obj : _objs)
			obj->update(delta);
}

void GameController::render()
{
	if (!_close)
	{
		_window.clear();
		for (GameObject* obj : _objs)
			obj->draw(&_window);
		_window.display();
	}
}

void GameController::processEvents()
{
	if (!_close)
	{
		sf::Event event;
		while (_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_close = true;
				return;
			}
			else _events.dispatchEvent(event);
		}
	}
}

void GameController::dispatchEvent(const InputEvent& event)
{
	for (GameObject* obj : _objs)
		obj->dispatchEvent(event);
}
