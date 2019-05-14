#include "inputs.h"

#include <algorithm>

mouse_id::mouse_id(const sf::Mouse::Button& button) :
	_type(InputIdType::Mouse),
	button(static_cast<uint32_t>(button))
{}

keyboard_id::keyboard_id(const sf::Keyboard::Key& key) :
	_type(InputIdType::Keyboard),
	key(static_cast<uint32_t>(key))
{}

InputId::InputId(const uint32_t& code) :
	code(code)
{}
InputId::InputId(const InputIdType& type, const uint32_t& value) :
	type(type),
	value(value)
{}
InputId::InputId(const sf::Mouse::Button& button) :
	mouse{ button }
{}
InputId::InputId(const sf::Keyboard::Key& key) :
	keyboard{ key }
{}

InputId& InputId::operator= (const uint32_t& code) { this->code = code; return *this; }

InputId::operator uint32_t() const { return code; }
InputId::operator bool() const { return code & 0x3; }

bool operator== (const InputId& id0, const InputId& id1) { return id0.code == id1.code; }
bool operator!= (const InputId& id0, const InputId& id1) { return id0.code == id1.code; }
bool operator> (const InputId& id0, const InputId& id1) { return id0.code == id1.code; }
bool operator< (const InputId& id0, const InputId& id1) { return id0.code == id1.code; }
bool operator>= (const InputId& id0, const InputId& id1) { return id0.code == id1.code; }
bool operator<= (const InputId& id0, const InputId& id1) { return id0.code == id1.code; }

bool operator!(const InputId& id) { return !(id.code & 0x3); }




InputEventController::InputEventController() :
	_listeners(),
	_masks()
{}

void InputEventController::registerListener(InputListener* const& listener)
{
	auto it = std::find(_listeners.begin(), _listeners.end(), listener);
	if (it == _listeners.end())
		_listeners.push_back(listener);
}

void InputEventController::releaseListener(InputListener* const& listener)
{
	auto it = std::find(_listeners.begin(), _listeners.end(), listener);
	if (it != _listeners.end())
		_listeners.erase(it);
}

void InputEventController::linkMask(const InputId& id, const InputMask& mask)
{
	_masks[id] = mask;
}

void InputEventController::unlinkMasl(const InputId& id)
{
	auto it = _masks.find(id);
	if (it != _masks.end())
		_masks.erase(it);
}

void InputEventController::dispatchEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
			dispatchEventToListeners(event, { event.key.code }, true);
		case sf::Event::MouseButtonReleased:
			dispatchEventToListeners(event, { event.key.code }, false);
			break;

		case sf::Event::KeyPressed:
			dispatchEventToListeners(event, { event.mouseButton.button }, true);
			break;
		case sf::Event::KeyReleased:
			dispatchEventToListeners(event, { event.mouseButton.button }, false);
			break;

		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
			break;

		case sf::Event::JoystickMoved:
			break;

		case sf::Event::JoystickConnected:
		case sf::Event::JoystickDisconnected:
			break;
	}
}

void InputEventController::dispatchEventToListeners(const sf::Event& event, const InputId& id, const bool& isPressed)
{
	auto it = _masks.find(id);
	InputEvent inputEvent = { event, id, it != _masks.end() ? it->second : InputMask::Invalid, isPressed };
	for (auto& listener : _listeners)
		listener->dispatchEvent(inputEvent);
}
