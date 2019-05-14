#pragma once

#include <map>

#include <SFML/Window.hpp>

enum class InputMask
{
	Invalid,

	P1_Fire,
	P1_Left,
	P1_Right,
	P1_Up,
	P1_Swap,
	P1_Slow,

	P2_Fire,
	P2_Left,
	P2_Right,
	P2_Up,
	P2_Swap,
	P2_Slow,

	Menu_Up,
	Menu_Down,
	Menu_Left,
	Menu_Right,
	Menu_Accept,
	Menu_Cancel
};

enum InputIdType : uint8_t
{
	Invalid,
	Keyboard,
	Mouse,
	Joystick
};

namespace
{
	struct mouse_id
	{
	private:
		uint32_t _type: 2;
	public:
		uint32_t button : 30;

		mouse_id(const sf::Mouse::Button& button);
	};

	struct keyboard_id
	{
	private:
		uint32_t _type: 2;
	public:
		uint32_t key : 30;

		keyboard_id(const sf::Keyboard::Key& key);
	};
}

union InputId
{
	uint32_t code;

	struct
	{
		uint32_t type : 2;
		uint32_t value : 30;
	};
	mouse_id mouse;
	keyboard_id keyboard;
	

	InputId(const uint32_t& code);
	InputId(const InputIdType& type, const uint32_t& value);
	InputId(const sf::Mouse::Button& button);
	InputId(const sf::Keyboard::Key& key);

	InputId& operator= (const uint32_t& code);

	operator uint32_t() const;
	operator bool() const;
};

bool operator== (const InputId& id0, const InputId& id1);
bool operator!= (const InputId& id0, const InputId& id1);
bool operator> (const InputId& id0, const InputId& id1);
bool operator< (const InputId& id0, const InputId& id1);
bool operator>= (const InputId& id0, const InputId& id1);
bool operator<= (const InputId& id0, const InputId& id1);

bool operator!(const InputId& id);


struct InputEvent
{
	const sf::Event& event;
	const InputId& id;
	const InputMask& mask;
	const bool& pressed;
};


class InputListener
{
public:
	virtual void dispatchEvent(const InputEvent& event) = 0;
};


class InputEventController
{
private:
	std::vector<InputListener*> _listeners;
	std::map<InputId, InputMask> _masks;

public:
	InputEventController();

	void registerListener(InputListener* const& listener);
	void releaseListener(InputListener* const& listener);

	void linkMask(const InputId& id, const InputMask& mask);
	void unlinkMasl(const InputId& id);

	void dispatchEvent(const sf::Event& event);

private:
	void dispatchEventToListeners(const sf::Event& event, const InputId& id, const bool& isPressed);
};

