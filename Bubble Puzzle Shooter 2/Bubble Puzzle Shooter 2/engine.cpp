#include "engine.h"

void Entity::draw(const sf::RenderTarget& g) {}
void Entity::update(const delta_t& delta) {}
void Entity::dispatchEvent(const sf::Event& event) {}




Frame::Frame(const std::string& tag) : Entity(tag), _entities() {}
Frame::Frame(const char* tag) : Entity(tag), _entities() {}
Frame::~Frame()
{
	if (!_entities.empty())
	{
		auto it = _entities.begin();
		while (it != _entities.end())
		{
			Entity* e = *it;
			_entities.erase(it++);
			delete e;
		}
		_entities.clear();
	}
}

void Frame::addEntity(Entity* e)
{
	if(!e)
		_entities.push_back(e);
}

void Frame::removeEntity(Entity* e)
{
	if (!e)
	{
		auto it = std::find(_entities.begin(), _entities.end(), e);
		if (it != _entities.end())
			_entities.erase(it);
	}
}

Entity* Frame::removeEntity(const uuid& id)
{
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity* e = *it;
		if (id == e->id())
		{
			_entities.erase(it);
			return e;
		}
	}
	return nullptr;
}

void Frame::clearEntities()
{
	_entities.clear();
}

Entity* Frame::findEntity(const uuid& id) const
{
	auto it = std::find_if(_entities.cbegin(), _entities.cend(), [&id](Entity *const e) { return id == e->id(); });
	return it != _entities.cend() ? *it : nullptr;
}

void Frame::forEachEntity(void(*action)(Entity *const e)) const
{
	for (Entity *const e : _entities)
		action(e);
}





GameController::GameController(const std::string& name)
	: _close(false), _window(), _entities(), _name(name), _vmode(640, 480), _vstyle(wstyle::Default),
	 _deltaClock(), _phAccumulator(), _phUps(sf::seconds(1.f / 60.f)) {}
GameController::~GameController()
{
	_close = true;
	if(_window.isOpen())
		_window.close();
	if (!_entities.empty())
	{
		auto it = _entities.begin();
		while (it != _entities.end())
		{
			auto e = *it;
			_entities.erase(it++);
			delete e;
		}
		_entities.clear();
	}
}


void GameController::start()
{
	init();
	loop();
}


void GameController::setVideoMode(sf::VideoMode mode, bool apply)
{
	_vmode = mode;
	if(apply)
		resetWindow();
}

void GameController::setStyle(wstyle style, bool apply)
{
	_vstyle = style;
	if (apply)
		resetWindow();
}


void GameController::addEntity(Entity* e)
{
	if (e->_gc == this)
		return;
	e->_gc = this;
	_entities.push_back(e);
}

void GameController::removeEntity(Entity* e)
{
	if (e->_gc != this)
		return;
	e->_gc = nullptr;
	auto it = std::find(_entities.begin(), _entities.end(), e);
	if (it != _entities.end())
		_entities.erase(it);
}

Entity* GameController::removeEntity(const uuid& id)
{
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity* e = *it;
		if (e->_id == id)
		{
			e->_gc = nullptr;
			_entities.erase(it);
			return e;
		}
	}
	return nullptr;
}

void GameController::deleteEntity(Entity* e)
{
	removeEntity(e);
	delete e;
}

void GameController::deleteEntity(const uuid& id)
{
	Entity* e = removeEntity(id);
	if (e != nullptr)
		delete e;
}


void GameController::loop()
{
	while (!_close)
	{
		processEvents();

		while (_phAccumulator > _phUps)
		{
			delta_t delta = _deltaClock.getElapsedTime().asSeconds();
			_phAccumulator -= _phUps;
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
	if (_close)
		return;

	for (auto e : _entities)
		e->update(delta);
}

void GameController::render()
{
	if (_close)
		return;

	_window.clear();
	for (auto e : _entities)
		e->draw(_window);
	_window.display();
}

void GameController::processEvents()
{
	if (_close)
		return;

	sf::Event event;
	while (_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_close = true;
			return;
		}
		for (auto e : _entities)
			e->dispatchEvent(event);
	}
}


void GameController::resetWindow()
{
	if (_close)
		return;

	if (_window.isOpen())
		_window.close();
	_window.create(_vmode, _name.c_str(), _vstyle);
}
