#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using int8  = sf::Int8;
using int16 = sf::Int16;
using int32 = sf::Int32;
using int64 = sf::Int64;

using uint8 =  sf::Uint8;
using uint16 = sf::Uint16;
using uint32 = sf::Uint32;
using uint64 = sf::Uint64;

using Pixel = uint32;

using Color = sf::Color;

using Vec2 = sf::Vector2f;
using Vec2f = Vec2;
using Vec2i = sf::Vector2i;
using Vec2u = sf::Vector2u;


/* UTILS */
class uuid
{
private:
	uint64 code;

	inline uuid(uint64 code) : code(code) {}

public:

	static uuid generate();

	inline bool operator== (const uuid& id) const { return code == id.code; }
	inline bool operator!= (const uuid& id) const { return code != id.code; }
	inline bool operator> (const uuid& id) const { return code > id.code; }
	inline bool operator>= (const uuid& id) const { return code >= id.code; }
	inline bool operator< (const uuid& id) const { return code < id.code; }
	inline bool operator<= (const uuid& id) const { return code <= id.code; }
	inline bool operator! () const { return !code; }

	friend std::ostream& operator<< (std::ostream&, uuid&);
	friend std::istream& operator>> (std::istream&, uuid&);
};

inline std::ostream& operator<< (std::ostream& os, uuid& id) { return os << id.code; }
inline std::istream& operator>> (std::istream& is, uuid& id) { return is >> id.code; }



/* EXCEPTIONS */
class Throwable
{
protected:
	const std::string message;
	const Throwable* cause;

public:
	Throwable(const std::string msg = "", const Throwable* cause = nullptr);
	~Throwable();

	inline std::string getMessage() const { return message; }

	inline bool hasCause() { return cause != nullptr; }
	inline const Throwable* getCause() const { return cause; }
};

class BPSException : public Throwable
{
public:
	BPSException(const std::string msg = "", const Throwable* cause = nullptr);
};
