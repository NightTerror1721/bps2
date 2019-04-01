#pragma once

#include <SFML/Graphics.hpp>
#include "utils.h"
#include "py.h"

#include <string>
#include <vector>

class GameController;

enum wstyle : uint32
{
	None = sf::Style::None,      ///< No border / title bar (this flag and all others are mutually exclusive)
	Titlebar = sf::Style::Titlebar, ///< Title bar + fixed border
	Resize = sf::Style::Resize, ///< Title bar + resizable border + maximize button
	Close = sf::Style::Close, ///< Title bar + close button
	Fullscreen = sf::Style::Fullscreen, ///< Fullscreen mode (this flag and all others are mutually exclusive)

	Default = sf::Style::Default ///< Default window style
};

class Entity
{
private:
	const uuid _id;
	std::string _tag;
	GameController *_gc;

protected:
	inline GameController* game_controller() { return _gc; }

public:
	inline Entity(const std::string& tag) : _id(uuid::generate()), _tag(tag), _gc(nullptr) {}
	inline Entity(const char* tag = "") : _id(uuid::generate()), _tag(tag), _gc(nullptr) {}

	inline const uuid& id() const { return _id; }

	inline const std::string& tag() const { return _tag; }
	inline void tag(const std::string& tag) { _tag = tag; }
	inline void tag(const char* tag) { _tag = tag; }

	inline bool hasAttached() { return _gc != nullptr; }

	virtual void draw(sf::RenderTarget *const (&g));
	virtual void update(const delta_t& delta);
	virtual void dispatchEvent(const sf::Event& event);

	friend class GameController;
};


/* PREDEFINED ENTITIES */

class Frame : public Entity
{
private:
	std::vector<Entity*> _entities;

public:
	Frame(const std::string& tag);
	Frame(const char* tag = "");
	~Frame();

	void addEntity(Entity* e);

	void removeEntity(Entity* e);
	Entity* removeEntity(const uuid& id);

	void clearEntities();

	Entity* findEntity(const uuid& id) const;

	void forEachEntity(void(*action)(Entity *const e)) const;

	inline size_t getEntityCount() const { return _entities.size(); }

	virtual void draw(sf::RenderTarget *const (&g)) override;
	virtual void update(const delta_t& delta) override;
	virtual void dispatchEvent(const sf::Event& event) override;
};

class PhysicalEntity : public Entity, public sf::Transformable
{
public:
	inline PhysicalEntity(const std::string& tag) : Entity(tag), Transformable() {}
	inline PhysicalEntity(const char* tag = "") : Entity(tag), Transformable() {}
};



/* GAME CONTROLLER */

class GameController
{
private:
	bool _close;
	sf::RenderWindow _window;
	std::vector<Entity*> _entities;

	py::scoped_interpreter _py;

	sf::Clock _deltaClock;
	sf::Time _phAccumulator;
	sf::Time _phUps;

	std::string _name;
	sf::VideoMode _vmode;
	wstyle _vstyle;



public:
	GameController(const std::string& name);
    ~GameController();

	void start();

	void setVideoMode(sf::VideoMode mode, bool apply = true);
	void setStyle(wstyle style, bool apply = true);
	void resetWindow();

	inline bool isFullscreen() { return (_vstyle & wstyle::Fullscreen) != 0; }

	void addEntity(Entity* e);
	void removeEntity(Entity* e);
	Entity* removeEntity(const uuid& id);
	void deleteEntity(Entity* e);
	void deleteEntity(const uuid& id);

	template<class __ETy = Entity>
	std::vector<Entity*> find(bool (*criteria)(const __ETy*))
	{
		std::vector<Entity*> vec;
		for (auto e : _entities)
			if (criteria(e))
				vec.push_back(e);
		return vec;
	}

private:

	void loop();

	void init();

	void update(delta_t);

	void render();

	void processEvents();
};
