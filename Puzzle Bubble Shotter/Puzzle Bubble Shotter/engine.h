#pragma once

#include <string>
#include <string>

#include <SFML/Graphics.hpp>

#include "py.h"
#include "mem_alloc.h"
#include "game_objs.h"

namespace wstyle = sf::Style;


class GameController : InputListener
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

	std::vector<GameObject*> _objs;

	InputEventController _events;


public:
	GameController(const std::string& name);
	~GameController();

	void start();

	void close();

	void setVideoMode(sf::VideoMode mode, bool apply = true);
	void setStyle(u32 style, bool apply = true);
	void resetWindow();

	bool isFullscreen() const;

	sf::RenderWindow* getWindow();
	const sf::RenderWindow* getWindow() const;

	template<class _Ty, class... _Args>
	GameObject* createGameObject(_Args&&... args)
	{
		GameObject* obj = new _Ty{ args... };
		obj->_gc = this;
		_objs.push_back(obj);
		return obj;
	}

	void destroyGameObject(GameObject* const& obj_ptr);

	void dispatchEvent(const InputEvent& event) override;

private:
	void loop();

	void init();
	void update(delta_t delta);
	void render();
	void processEvents();
};
