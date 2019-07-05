#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <type_traits>
#include <SFML/System/Vector2.hpp>

#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi

#define RAD_2_DEG(x) ((x) * 180.f / M_PI)
#define DEG_2_RAD(x) ((x) * M_PI / 180.f)

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

	virtual bool operator== (const UniqueObject& uo) const;
	virtual bool operator!= (const UniqueObject& uo) const;
};


class Singleton
{
protected:
	Singleton();
	~Singleton();

private:
	Singleton(const Singleton&);
	Singleton(Singleton&&);

	Singleton& operator= (const Singleton&);
	Singleton& operator= (Singleton&&);
};

#define RANGE_LIMIT(minValue, maxValue, value) __min((maxValue), (__max((minValue), (value))))


class Config;
Config& __ConfigInstance();

class Config : public Singleton
{
private:
	enum class DataType : u8 { Null, Integer, Float, Boolean, String };
	struct Data
	{
		DataType type;
		void* data;

		friend class Config;

		Data();
		~Data();

	private:
		void setNull(const std::nullptr_t&);
		void setInteger(const int32& value);
		void setFloat(const float& value);
		void setBoolean(const bool& value);
		void setString(const std::string& value);

		bool isNull() const;
		int32 getInteger(int32 default_value = 0) const;
		float getFloat(float default_value = 0.f) const;
		bool getBoolean(bool default_value = false) const;
		const std::string& getString(const std::string& default_value = "") const;
	};

private:
	static Config _instance;

	std::map<std::string, Data> _map;

	Config();

public:
	void setNull(const std::string& key);
	void setInteger(const std::string& key, int32 value);
	void setFloat(const std::string& key, float value);
	void setBoolean(const std::string& key, bool value);
	void setString(const std::string& key, const std::string& value);

	static bool isNull(const std::string& key);
	static int32 getInteger(const std::string& key, int32 default_value = 0);
	static float getFloat(const std::string& key, float default_value = 0.f);
	static bool getBoolean(const std::string& key, bool default_value = false);
	static const std::string& getString(const std::string& key, const std::string& default_value = "");

	friend Config& __ConfigInstance();

private:
	template<class _Ty>
	void _InsertValue(const std::string& key, const _Ty& value, void (Data::*setFunction)(const _Ty&))
	{
		auto it = _map.find(key);
		if (it == _map.end())
		{
			auto eit = _map.emplace(std::pair<std::string, Data>{ key, {} });
			if (eit.second)
				(eit.first->second.*setFunction)(value);
		}
		else (it->second.*setFunction)(value);
	}
};
