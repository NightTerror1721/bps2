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

template<class _Base, _Base _Min, _Base _Max>
class LimitedValue
{
private:
	_Base _value;

public:
	inline LimitedValue() : _value{ _Min } {}
	inline LimitedValue(const _Base& value) : _value{ RANGE_LIMIT(_Min, _Max, value) } {}

	inline LimitedValue& operator= (const _Base& value) { _value = RANGE_LIMIT(_Min, _Max, value); return *this; }

	inline LimitedValue& operator+= (const LimitedValue& other) { _value = RANGE_LIMIT(_Min, _Max, (_value + other._value)); return *this; }
	inline LimitedValue& operator-= (const LimitedValue& other) { _value = RANGE_LIMIT(_Min, _Max, (_value - other._value)); return *this; }
	inline LimitedValue& operator*= (const LimitedValue& other) { _value = RANGE_LIMIT(_Min, _Max, (_value * other._value)); return *this; }
	inline LimitedValue& operator/= (const LimitedValue& other) { _value = RANGE_LIMIT(_Min, _Max, (_value / other._value)); return *this; }
	inline LimitedValue& operator%= (const LimitedValue& other) { _value = RANGE_LIMIT(_Min, _Max, (_value % other._value)); return *this; }
	inline LimitedValue& operator+= (const _Base& value) { _value = RANGE_LIMIT(_Min, _Max, (_value + value)); return *this; }
	inline LimitedValue& operator-= (const _Base& value) { _value = RANGE_LIMIT(_Min, _Max, (_value - value)); return *this; }
	inline LimitedValue& operator*= (const _Base& value) { _value = RANGE_LIMIT(_Min, _Max, (_value * value)); return *this; }
	inline LimitedValue& operator/= (const _Base& value) { _value = RANGE_LIMIT(_Min, _Max, (_value / value)); return *this; }
	inline LimitedValue& operator%= (const _Base& value) { _value = RANGE_LIMIT(_Min, _Max, (_value % value)); return *this; }
	inline LimitedValue& operator+= (const int& value) { _value = RANGE_LIMIT(_Min, _Max, (_value + static_cast<_Base>(value))); return *this; }
	inline LimitedValue& operator-= (const int& value) { _value = RANGE_LIMIT(_Min, _Max, (_value - static_cast<_Base>(value))); return *this; }
	inline LimitedValue& operator*= (const int& value) { _value = RANGE_LIMIT(_Min, _Max, (_value * static_cast<_Base>(value))); return *this; }
	inline LimitedValue& operator/= (const int& value) { _value = RANGE_LIMIT(_Min, _Max, (_value / static_cast<_Base>(value))); return *this; }
	inline LimitedValue& operator%= (const int& value) { _value = RANGE_LIMIT(_Min, _Max, (_value % static_cast<_Base>(value))); return *this; }

	inline LimitedValue operator+ (const LimitedValue& other) const { return { RANGE_LIMIT(_Min, _Max, (_value + other._value)) }; }
	inline LimitedValue operator- (const LimitedValue& other) const { return { RANGE_LIMIT(_Min, _Max, (_value - other._value)) }; }
	inline LimitedValue operator* (const LimitedValue& other) const { return { RANGE_LIMIT(_Min, _Max, (_value * other._value)) }; }
	inline LimitedValue operator/ (const LimitedValue& other) const { return { RANGE_LIMIT(_Min, _Max, (_value / other._value)) }; }
	inline int operator% (const LimitedValue& other) const { return _value % other._value; }
	inline LimitedValue operator+ (const _Base& value) const { return { RANGE_LIMIT(_Min, _Max, (_value + value)) }; }
	inline LimitedValue operator- (const _Base& value) const { return { RANGE_LIMIT(_Min, _Max, (_value - value)) }; }
	inline LimitedValue operator* (const _Base& value) const { return { RANGE_LIMIT(_Min, _Max, (_value * value)) }; }
	inline LimitedValue operator/ (const _Base& value) const { return { RANGE_LIMIT(_Min, _Max, (_value / value)) }; }
	inline int operator% (const _Base& value) const { return _value % value; }
	inline LimitedValue operator+ (const int& value) const { return { static_cast<_Base>(value + *this) }; }
	inline LimitedValue operator- (const int& value) const { return { static_cast<_Base>(value - *this) }; }
	inline LimitedValue operator* (const int& value) const { return { static_cast<_Base>(value * *this) }; }
	inline LimitedValue operator/ (const int& value) const { return { static_cast<_Base>(value / *this) }; }
	inline int operator% (const int& value) const { return _value % (int) value; }

	inline LimitedValue& operator++ () { _value = RANGE_LIMIT(_Min, _Max, (_value + 1)); return *this; }
	inline LimitedValue operator++ (int) { LimitedValue old{ *this }; operator++(); return std::move(old); }

	inline LimitedValue& operator-- () { _value = RANGE_LIMIT(_Min, _Max, (_value - 1)); return *this; }
	inline LimitedValue operator-- (int) { LimitedValue old{ *this }; operator--(); return std::move(old); }

	inline bool operator== (const LimitedValue& other) const { return _value == other._value; }
	inline bool operator!= (const LimitedValue& other) const { return _value != other._value; }
	inline bool operator> (const LimitedValue& other) const { return _value > other._value; }
	inline bool operator< (const LimitedValue& other) const { return _value < other._value; }
	inline bool operator>= (const LimitedValue& other) const { return _value >= other._value; }
	inline bool operator<= (const LimitedValue& other) const { return _value <= other._value; }
	inline bool operator== (const _Base& value) const { return _value == value; }
	inline bool operator!= (const _Base& value) const { return _value != value; }
	inline bool operator> (const _Base& value) const { return _value > value; }
	inline bool operator< (const _Base& value) const { return _value < value; }
	inline bool operator>= (const _Base& value) const { return _value >= value; }
	inline bool operator<= (const _Base& value) const { return _value <= value; }
	inline bool operator== (const int& value) const { return _value == static_cast<_Base>(value); }
	inline bool operator!= (const int& value) const { return _value != static_cast<_Base>(value); }
	inline bool operator> (const int& value) const { return _value > static_cast<_Base>(value); }
	inline bool operator< (const int& value) const { return _value < static_cast<_Base>(value); }
	inline bool operator>= (const int& value) const { return _value >= static_cast<_Base>(value); }
	inline bool operator<= (const int& value) const { return _value <= static_cast<_Base>(value); }

	inline bool operator! () const { return _value == _Min; }
	inline operator bool() const { return _value != _Min; }

	inline operator _Base() { return _value; }

	inline explicit operator float() { return static_cast<float>(_value); }
	inline explicit operator double() { return static_cast<double>(_value); }
	inline explicit operator long double() { return static_cast<long double>(_value); }

	static constexpr _Base min() { return _Min; }
	static constexpr _Base max() { return _Max; }
	static constexpr _Base base(const LimitedValue& value) { return value._value; }
	static constexpr _Base filter(const _Base& value) { return static_cast<_Base>(LimitedValue{ value }); }

	template<class _Base, _Base _Min, _Base _Max>
	friend inline _Base operator+ (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline _Base operator- (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline _Base operator* (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline _Base operator/ (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline _Base operator% (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline int operator+ (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline int operator- (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline int operator* (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline int operator/ (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline int operator% (const int& base, const LimitedValue<_Base, _Min, _Max>& value);

	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator== (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator!= (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator> (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator< (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator>= (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator<= (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator== (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator!= (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator> (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator< (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator>= (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
	template<class _Base, _Base _Min, _Base _Max>
	friend inline bool operator<= (const int& base, const LimitedValue<_Base, _Min, _Max>& value);
};

template<class _Base, _Base _Min, _Base _Max>
inline _Base operator+ (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base + value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline _Base operator- (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base - value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline _Base operator* (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base * value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline _Base operator/ (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base / value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline _Base operator% (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base % value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline int operator+ (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base + static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline int operator- (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base - static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline int operator* (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base * static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline int operator/ (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base / static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline int operator% (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base % static_cast<int>(value._value); }

template<class _Base, _Base _Min, _Base _Max>
inline bool operator== (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base == value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator!= (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base != value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator> (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base > value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator< (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base < value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator>= (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base >= value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator<= (const _Base& base, const LimitedValue<_Base, _Min, _Max>& value) { return base <= value._value; }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator== (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base == static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator!= (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base != static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator> (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base > static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator< (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base < static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator>= (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base >= static_cast<int>(value._value); }
template<class _Base, _Base _Min, _Base _Max>
inline bool operator<= (const int& base, const LimitedValue<_Base, _Min, _Max>& value) { return base <= static_cast<int>(value._value); }

