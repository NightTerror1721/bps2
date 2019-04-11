#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>

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

using delta_t = float;

using std::byte;


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

#define minmax_range(_min, _max, __val) std::min((_min), std::max((_max), (__val)))

template<uuid (*__IdGen)() = &uuid::generate>
class Unique
{
private:
	const uuid _id;

public:
	inline Unique() : _id(__IdGen()) {}

	inline uuid getUniqueId() const { return _id; }

	inline bool operator== (const Unique& other) const { return _id == other._id; }
	inline bool operator!= (const Unique& other) const { return _id != other._id; }
};

template<class __Ty>
class DynMatrix2
{
private:
	using T = __Ty;

	std::vector<T> _data;
	const size_t _rows;
	const size_t _cols;
	const size_t _size;

public: 
	class MatrixRow
	{
	private:
		DynMatrix2* const& __mat;
		const size_t& _row;

	public:
		__forceinline MatrixRow(DynMatrix2* const& mat, const size_t& row) : __mat(mat), _row(row) {}
		__forceinline T& operator[](const size_t& column) { return __mat->_data[_row + column]; }
		//__forceinline const T& operator[](const size_t& column) const { return __mat->_data[_row + column]; }
	};

	class ConstMatrixRow
	{
	private:
		const DynMatrix2* const& __mat;
		const size_t& _row;

	public:
		__forceinline ConstMatrixRow(const DynMatrix2* const& mat, const size_t& row) : __mat(mat), _row(row) {}
		//__forceinline T& operator[](const size_t& column) { return __mat->_data[_row + column]; }
		__forceinline const T& operator[](const size_t& column) const { return __mat->_data[_row + column]; }
	};

public:
	inline DynMatrix2(const size_t& rows, const size_t& columns) : _rows(rows), _cols(columns), _size(_rows * _cols), _data(_size) {}
	DynMatrix2() = delete;
	//DynMatrix2(const DynMatrix2&) = delete;
	~DynMatrix2()
	{
		_data.clear();
	}

	DynMatrix2& operator= (const DynMatrix2&) = delete;

	inline size_t rows() { return _rows; }
	inline size_t columns() { return _cols; }
	inline size_t matrix_size() { return _size; }

	inline void fill(const T& value) { _data.assign(_size, value); }

	__forceinline MatrixRow      operator[](const size_t& row) { return { this, row / _cols }; }
	__forceinline ConstMatrixRow operator[](const size_t& row) const { return { this, row / _cols }; }

	__forceinline T& operator() (const size_t& idx) { return _data[idx]; }

};

__forceinline byte operator"" _b(unsigned long long int value) { return static_cast<byte>(value & 0xff); }



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

class singleton
{
protected:
	singleton() {}
	~singleton() {}

private:
	singleton(const singleton&);
	singleton(singleton&&);

	singleton& operator= (const singleton&);
	singleton& operator= (singleton&&);
};

#define init_vector(vec, len, statement) (vec).reserve((len)); for(unsigned __i__=0;__i__<(len);__i__++) (vec).emplace_back((statement))



class LocalAttrAllocator
{
private:
	std::unordered_map<std::string, int32> _ints;
	std::unordered_map<std::string, float> _flts;
	std::unordered_map<std::string, std::string> _strs;

public:

	void  setLocalInt(const std::string& name, const int32& value);
	int32 getLocalInt(const std::string& name, const int32& default_value = 0);
	bool  deleteLocalInt(const std::string& name);

	void  setLocalFloat(const std::string& name, const float& value);
	float getLocalFloat(const std::string& name, const float& default_value = 0.f);
	bool  deleteLocalFloat(const std::string& name);

	void  setLocalString(const std::string& name, const std::string& value);
	std::string getLocalString(const std::string& name, const std::string& default_value = "");
	bool  deleteLocalString(const std::string& name);

	void deleteAllLocals();

private:
	template<class __Ty>
	using map_t = std::unordered_map<std::string, __Ty>;

	template<class __Ty>
	__forceinline void setLocal(map_t<__Ty>& map, const std::string& name, const __Ty& value) { map[name] = value; }

	template<class __Ty>
	__forceinline __Ty getLocal(map_t<__Ty>& map, const std::string& name, const __Ty& default_value)
	{
		auto it = map.find(name);
		return it == map.end() ? default_value : it->second;
	}

	template<class __Ty>
	__forceinline bool deleteLocal(map_t<__Ty>& map, const std::string& name)
	{
		auto it = map.find(name);
		if (it != map.end())
		{
			map.erase(it);
			return true;
		}
		return false;
	}
};






#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi

#define RAD_2_DEG(x) ((x) * 180.f / M_PI)
#define DEG_2_RAD(x) ((x) * M_PI / 180.f)

/*class degrees_t;
class radians_t;

class degrees_t
{
private:
	float _value;

public:
	inline degrees_t() : _value(0.f) {}
	inline degrees_t(const float& value) : _value(fix(value)) {}
	degrees_t(const radians_t&);

	degrees_t& operator= (const float&);
	degrees_t& operator= (const radians_t&);

	operator float() const;

	radians_t radians() const;

	friend class radians_t;

	inline bool operator== (const degrees_t& v2) const { return _value == v2._value; }
	inline bool operator== (const radians_t& v2) const { return radians_t{ *this }._value == v2._value; }
	inline bool operator== (const float& v2) const { return _value == v2; }
	friend bool operator== (const float& v1, const degrees_t& v2);

	inline bool operator!= (const degrees_t& v2) const { return _value != v2._value; }
	inline bool operator!= (const radians_t& v2) const { return radians_t{ *this }._value != v2._value; }
	inline bool operator!= (const float& v2) const { return _value != v2; }
	friend bool operator!= (const float& v1, const degrees_t& v2);

	inline bool operator> (const degrees_t& v2) const { return _value > v2._value; }
	inline bool operator> (const radians_t& v2) const { return radians_t{ *this }._value > v2._value; }
	inline bool operator> (const float& v2) const { return _value > v2; }
	friend bool operator> (const float& v1, const degrees_t& v2);

	inline bool operator< (const degrees_t& v2) const { return _value < v2._value; }
	inline bool operator< (const radians_t& v2) const { return radians_t{ *this }._value < v2._value; }
	inline bool operator< (const float& v2) const { return _value < v2; }
	friend bool operator< (const float& v1, const degrees_t& v2);

	inline bool operator>= (const degrees_t& v2) const { return _value >= v2._value; }
	inline bool operator>= (const radians_t& v2) const { return radians_t{ *this }._value >= v2._value; }
	inline bool operator>= (const float& v2) const { return _value >= v2; }
	friend bool operator>= (const float& v1, const degrees_t& v2);

	inline bool operator<= (const degrees_t& v2) const { return _value <= v2._value; }
	inline bool operator<= (const radians_t& v2) const { return radians_t{ *this }._value <= v2._value; }
	inline bool operator<= (const float& v2) const { return _value <= v2; }
	friend bool operator<= (const float& v1, const degrees_t& v2);

	inline degrees_t operator+ (const degrees_t& v2) const { return _value + v2._value; }
	inline degrees_t operator+ (const radians_t& v2) const { return _value + degrees_t{ v2 }._value; }
	inline degrees_t operator+ (const float& v2) const { return _value + v2; }
	friend degrees_t operator+ (const float& v1, const degrees_t& v2);

	inline degrees_t operator- (const degrees_t& v2) const { return _value - v2._value; }
	inline degrees_t operator- (const radians_t& v2) const { return _value - degrees_t{ v2 }._value; }
	inline degrees_t operator- (const float& v2) const { return _value - v2; }
	friend degrees_t operator- (const float& v1, const degrees_t& v2);

	inline degrees_t operator* (const degrees_t& v2) const { return _value * v2._value; }
	inline degrees_t operator* (const radians_t& v2) const { return _value * degrees_t{ v2 }._value; }
	inline degrees_t operator* (const float& v2) const { return _value * v2; }
	friend degrees_t operator* (const float& v1, const degrees_t& v2);

	inline degrees_t operator/ (const degrees_t& v2) const { return _value / v2._value; }
	inline degrees_t operator/ (const radians_t& v2) const { return _value / degrees_t{ v2 }._value; }
	inline degrees_t operator/ (const float& v2) const { return _value / v2; }
	friend degrees_t operator/ (const float& v1, const degrees_t& v2);

	degrees_t& operator+= (const degrees_t& v2);
	degrees_t& operator+= (const radians_t& v2);
	degrees_t& operator+= (const float& v2);

	degrees_t& operator-= (const degrees_t& v2);
	degrees_t& operator-= (const radians_t& v2);
	degrees_t& operator-= (const float& v2);

	degrees_t& operator*= (const degrees_t& v2);
	degrees_t& operator*= (const radians_t& v2);
	degrees_t& operator*= (const float& v2);

	degrees_t& operator/= (const degrees_t& v2);
	degrees_t& operator/= (const radians_t& v2);
	degrees_t& operator/= (const float& v2);


private:
	__forceinline static float fix(float value)
	{
		if (value >= 360.f)
		{
			int base = static_cast<int>(value / 360.f);
			return value - (360.f * base);
		}
		else if (value < 0)
		{
			int base = static_cast<int>(-value / 360.f);
			value = value + (360.f * base) + 360.f;
			return value == 360.f ? 0.f : value;
		}
		return value;
	}
};

class radians_t
{
private:
	float _value;

public:
	inline radians_t() : _value(0.f) {}
	inline radians_t(const float& value) : _value(value) {}
	radians_t(const degrees_t&);

	radians_t& operator= (const float&);
	radians_t& operator= (const degrees_t&);

	operator float() const;

	degrees_t degrees() { return _value; }

	friend class degrees_t;

	inline bool operator== (const radians_t& v2) const { return _value == v2._value; }
	inline bool operator== (const degrees_t& v2) const { return _value == radians_t{ v2 }._value; }
	inline bool operator== (const float& v2) const { return _value == v2; }
	friend bool operator== (const float& v1, const radians_t& v2);

	inline bool operator!= (const radians_t& v2) const { return _value != v2._value; }
	inline bool operator!= (const degrees_t& v2) const { return _value != radians_t{ v2 }._value; }
	inline bool operator!= (const float& v2) const { return _value != v2; }
	friend bool operator!= (const float& v1, const radians_t& v2);

	inline bool operator> (const radians_t& v2) const { return _value > v2._value; }
	inline bool operator> (const degrees_t& v2) const { return _value > radians_t{ v2 }._value; }
	inline bool operator> (const float& v2) const { return _value > v2; }
	friend bool operator> (const float& v1, const radians_t& v2);

	inline bool operator< (const radians_t& v2) const { return _value < v2._value; }
	inline bool operator< (const degrees_t& v2) const { return _value < radians_t{ v2 }._value; }
	inline bool operator< (const float& v2) const { return _value < v2; }
	friend bool operator< (const float& v1, const radians_t& v2);

	inline bool operator>= (const radians_t& v2) const { return _value >= v2._value; }
	inline bool operator>= (const degrees_t& v2) const { return _value >= radians_t{ v2 }._value; }
	inline bool operator>= (const float& v2) const { return _value >= v2; }
	friend bool operator>= (const float& v1, const radians_t& v2);

	inline bool operator<= (const radians_t& v2) const { return _value <= v2._value; }
	inline bool operator<= (const degrees_t& v2) const { return _value <= radians_t{ v2 }._value; }
	inline bool operator<= (const float& v2) const { return _value <= v2; }
	friend bool operator<= (const float& v1, const radians_t& v2);

	inline radians_t operator+ (const radians_t& v2) const { return _value + v2._value; }
	inline radians_t operator+ (const degrees_t& v2) const { return _value + radians_t{ v2 }._value; }
	inline radians_t operator+ (const float& v2) const { return _value + v2; }
	friend radians_t operator+ (const float& v1, const radians_t& v2);

	inline radians_t operator- (const radians_t& v2) const { return _value - v2._value; }
	inline radians_t operator- (const degrees_t& v2) const { return _value - radians_t{ v2 }._value; }
	inline radians_t operator- (const float& v2) const { return _value - v2; }
	friend radians_t operator- (const float& v1, const radians_t& v2);

	inline radians_t operator* (const radians_t& v2) const { return _value * v2._value; }
	inline radians_t operator* (const degrees_t& v2) const { return _value * radians_t{ v2 }._value; }
	inline radians_t operator* (const float& v2) const { return _value * v2; }
	friend radians_t operator* (const float& v1, const radians_t& v2);

	inline radians_t operator/ (const radians_t& v2) const { return _value / v2._value; }
	inline radians_t operator/ (const degrees_t& v2) const { return _value / radians_t{ v2 }._value; }
	inline radians_t operator/ (const float& v2) const { return _value / v2; }
	friend radians_t operator/ (const float& v1, const radians_t& v2);

	radians_t& operator+= (const radians_t& v2);
	radians_t& operator+= (const degrees_t& v2);
	radians_t& operator+= (const float& v2);

	radians_t& operator-= (const radians_t& v2);
	radians_t& operator-= (const degrees_t& v2);
	radians_t& operator-= (const float& v2);

	radians_t& operator*= (const radians_t& v2);
	radians_t& operator*= (const degrees_t& v2);
	radians_t& operator*= (const float& v2);

	radians_t& operator/= (const radians_t& v2);
	radians_t& operator/= (const degrees_t& v2);
	radians_t& operator/= (const float& v2);
};


inline bool operator== (const float& v1, const degrees_t& v2) { return v1 == v2._value; }
inline bool operator!= (const float& v1, const degrees_t& v2) { return v1 != v2._value; }
inline bool operator> (const float& v1, const degrees_t& v2) { return v1 > v2._value; }
inline bool operator< (const float& v1, const degrees_t& v2) { return v1 < v2._value; }
inline bool operator>= (const float& v1, const degrees_t& v2) { return v1 >= v2._value; }
inline bool operator<= (const float& v1, const degrees_t& v2) { return v1 <= v2._value; }
inline degrees_t operator+ (const float& v1, const degrees_t& v2) { return v1 + v2._value; }
inline degrees_t operator- (const float& v1, const degrees_t& v2) { return v1 - v2._value; }
inline degrees_t operator* (const float& v1, const degrees_t& v2) { return v1 * v2._value; }
inline degrees_t operator/ (const float& v1, const degrees_t& v2) { return v1 / v2._value; }

inline bool operator== (const float& v1, const radians_t& v2) { return v1 == v2._value; }
inline bool operator!= (const float& v1, const radians_t& v2) { return v1 != v2._value; }
inline bool operator> (const float& v1, const radians_t& v2) { return v1 > v2._value; }
inline bool operator< (const float& v1, const radians_t& v2) { return v1 < v2._value; }
inline bool operator>= (const float& v1, const radians_t& v2) { return v1 >= v2._value; }
inline bool operator<= (const float& v1, const radians_t& v2) { return v1 <= v2._value; }
inline radians_t operator+ (const float& v1, const radians_t& v2) { return v1 + v2._value; }
inline radians_t operator- (const float& v1, const radians_t& v2) { return v1 - v2._value; }
inline radians_t operator* (const float& v1, const radians_t& v2) { return v1 * v2._value; }
inline radians_t operator/ (const float& v1, const radians_t& v2) { return v1 / v2._value; }

#define degrees(value) degrees_t{ (value) }
#define radians(value) radians_t{ (value) }


inline degrees_t operator"" _deg(long double value) { return value; }
inline degrees_t operator"" _deg(unsigned long long int value) { return value; }

inline radians_t operator"" _rad(long double value) { return value; }
inline radians_t operator"" _rad(unsigned long long int value) { return value; }*/
