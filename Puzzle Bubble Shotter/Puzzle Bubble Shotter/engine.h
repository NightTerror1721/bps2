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

	void setVideoMode(sf::VideoMode mode, bool apply = true);
	void setStyle(u32 style, bool apply = true);
	void resetWindow();

	bool isFullscreen() const;

	template<class... _Args>
	Ptr<GameObject> createGameObject(_Args&&... args)
	{
		return _objs.create(args...);
	}

	void destroyGameObject(Ptr<GameObject> obj_ptr);

	Ptr<GameObject> findGameObject(const UUID& id);
	const Ptr<GameObject> findGameObject(const UUID& id) const;

	template<class _GoTy = GameObject>
	void forEachGameObject(void(*action)(_GoTy&))
	{

	}
};
