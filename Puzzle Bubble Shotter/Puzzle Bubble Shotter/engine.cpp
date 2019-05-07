#include "engine.h"

#include <SFML/OpenGL.hpp>

GameController::GameController(const std::string& name) :
	_close(false),
	_window(),
	_py(),
	_deltaClock(),
	_phAccumulator(),
	_phUps(sf::seconds(1.f / 60.f)),
	_name(name),
	_vmode(640, 480),
	_wstyle(wstyle::Default),
	_objs()
{}
GameController::~GameController() {}

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

void GameController::destroyGameObject(Ptr<GameObject> obj_ptr) { _objs.destroy(obj_ptr); }

std::vector<Ptr<GameObject>> GameController::findGameObject(std::function<bool(const GameObject&)> criteria) { return _objs.find(criteria); }

void GameController::forEachGameObject(std::function<void(GameObject&)> action) { _objs.forEach(action); }
void GameController::forEachGameObject(std::function<void(const GameObject&)> action) const { _objs.forEach(action); }


void GameController::loop()
{
	while (!_close)
	{
		processEvents();

		while (_phAccumulator > _phUps)
		{
			delta_t delta = _deltaClock.getElapsedTime().asSeconds();
			_phAccumulator -= _phUps;
			_deltaClock.restart();
			update(delta);
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
		for (GameObject& obj : _objs)
			obj.update(delta);
}

void GameController::render()
{
	if (!_close)
	{
		_window.clear();
		for (GameObject& obj : _objs)
			obj.draw(&_window);
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
			for (GameObject& obj : _objs)
				obj.dispatchEvent(event);
		}
	}
}
