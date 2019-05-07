#pragma once

#include <string>
#include <string>

#include <SFML/Graphics.hpp>

#include "py.h"
#include "mem_alloc.h"
#include "game_objs.h"

namespace wstyle = sf::Style;


class GameController
{
private:
	bool _close;
	sf::RenderWindow _window;

	py::scoped_interpreter _py;

	sf::Clock _deltaClock;
	sf::Time _phAccumulator;
	sf::Time _phUps;

	std::string _name;
	sf::VideoMode _vmode;
	u32 _wstyle;

	MemoryAllocator<GameObject> _objs;


public:
	GameController(const std::string& name);
	~GameController();

	void start();

	void close();

	void setVideoMode(sf::VideoMode mode, bool apply = true);
	void setStyle(u32 style, bool apply = true);
	void resetWindow();

	bool isFullscreen() const;

	template<class... _Args>
	Ptr<GameObject> createGameObject(_Args&&... args)
	{
		Ptr<GameObject> obj = _objs.create(args...);
		obj->_gc = this;
		return obj;
	}

	void destroyGameObject(Ptr<GameObject> obj_ptr);

	std::vector<Ptr<GameObject>> findGameObject(std::function<bool(const GameObject&)> criteria);

	void forEachGameObject(std::function<void(GameObject&)> action);
	void forEachGameObject(std::function<void(const GameObject&)> action) const;

private:
	void loop();

	void init();
	void update(delta_t delta);
	void render();
	void processEvents();
};
