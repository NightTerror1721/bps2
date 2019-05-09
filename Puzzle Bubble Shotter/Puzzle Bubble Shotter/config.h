#pragma once

#include <iostream>
#include <type_traits>
#include <SFML/System/Vector2.hpp>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float delta_t;

typedef sf::Vector2f vec2;
typedef sf::Vector2f vec2f;
typedef sf::Vector2i vec2i;
typedef sf::Vector2u vec2u;

using std::size_t;


template<class _Ty>
class MemoryAllocator;

class UniqueID
{
private:
	u64 _code;
	static u64 __gen;

public:
	static UniqueID generate();

	bool operator== (const UniqueID& uuid) const;
	bool operator!= (const UniqueID& uuid) const;
	bool operator> (const UniqueID& uuid) const;
	bool operator< (const UniqueID& uuid) const;
	bool operator>= (const UniqueID& uuid) const;
	bool operator<= (const UniqueID& uuid) const;

	friend std::ostream& operator<< (std::ostream& os, const UniqueID& uuid);
	friend std::istream& operator>> (std::istream& is, UniqueID& uuid);

	friend class UUID_Hash;
	template<class _Ty> friend class MemoryAllocator;

private:
	UniqueID(u64 code);
};

std::ostream& operator<< (std::ostream& os, const UniqueID& uuid);
std::istream& operator>> (std::istream& is, UniqueID& uuid);

class UUID_Hash
{
public:
	size_t operator() (const UniqueID& uuid) const
	{
		return std::hash<u64>()(uuid._code);
	}
};


class UniqueObject
{
private:
	const UniqueID _id;

public:
	explicit UniqueObject();

	const UniqueID& uuid() const;

	virtual bool operator== (const UniqueObject& uo);
	virtual bool operator!= (const UniqueObject& uo);
};

