#include "config.h"

#include <string>

u64 UniqueID::__gen;

UniqueID::UniqueID(u64 code) :
	_code(code)
{}

UniqueID UniqueID::generate()
{
	return { ++__gen };
}

bool UniqueID::operator== (const UniqueID& uuid) const { return _code == uuid._code; }
bool UniqueID::operator!= (const UniqueID& uuid) const { return _code != uuid._code; }
bool UniqueID::operator> (const UniqueID& uuid) const { return _code > uuid._code; }
bool UniqueID::operator< (const UniqueID& uuid) const { return _code < uuid._code; }
bool UniqueID::operator>= (const UniqueID& uuid) const { return _code >= uuid._code; }
bool UniqueID::operator<= (const UniqueID& uuid) const { return _code <= uuid._code; }

std::ostream& operator<< (std::ostream& os, const UniqueID& uuid) { return os << uuid._code; }
std::istream& operator>> (std::istream& is, UniqueID& uuid) { return is >> uuid._code; }



UniqueObject::UniqueObject() :
	_id(UniqueID::generate())
{}

const UniqueID& UniqueObject::uuid() const { return _id; }

bool UniqueObject::operator== (const UniqueObject& uo) const { return _id == uo._id; }
bool UniqueObject::operator!= (const UniqueObject& uo) const { return _id != uo._id; }



Singleton::Singleton() {}
Singleton::~Singleton() {}



#define delete_data() if(this->data) delete data

Config::Data::Data() :
	type{ Config::DataType::Null },
	data{ nullptr }
{}
Config::Data::~Data()
{
	delete_data();
}

void Config::Data::setNull(const std::nullptr_t&) { delete_data(); type = Config::DataType::Null; data = nullptr; }
void Config::Data::setInteger(const int32& value)
{
	delete_data();
	type = Config::DataType::Integer;
	data = new int32{ value };
}
void Config::Data::setFloat(const float& value)
{
	delete_data();
	type = Config::DataType::Float;
	data = new float{ value };
}
void Config::Data::setBoolean(const bool& value)
{
	delete_data();
	type = Config::DataType::Boolean;
	data = new bool{ value };
}
void Config::Data::setString(const std::string& value)
{
	delete_data();
	type = Config::DataType::String;
	data = new std::string{ value };
}

bool Config::Data::isNull() const
{
	return type == Config::DataType::Null;
}
int32 Config::Data::getInteger(int32 default_value) const
{
	switch (type)
	{
		case Config::DataType::Null: return default_value;
		case Config::DataType::Integer: return *reinterpret_cast<int32*>(data);
		case Config::DataType::Float: return static_cast<int32>(*reinterpret_cast<float*>(data));
		case Config::DataType::Boolean: return static_cast<int32>(*reinterpret_cast<bool*>(data));
		case Config::DataType::String:
			try { return std::stoi(*reinterpret_cast<std::string*>(data)); }
			catch (...) { return default_value; }
	}
	return default_value;
}
float Config::Data::getFloat(float default_value) const
{
	switch (type)
	{
		case Config::DataType::Null: return default_value;
		case Config::DataType::Integer: return static_cast<float>(*reinterpret_cast<int32*>(data));
		case Config::DataType::Float: return *reinterpret_cast<float*>(data);
		case Config::DataType::Boolean: return static_cast<float>(*reinterpret_cast<bool*>(data));
		case Config::DataType::String:
			try { return std::stof(*reinterpret_cast<std::string*>(data)); }
			catch (...) { return default_value; }
	}
	return default_value;
}
bool Config::Data::getBoolean(bool default_value) const
{
	switch (type)
	{
		case Config::DataType::Null: return default_value;
		case Config::DataType::Integer: return static_cast<bool>(*reinterpret_cast<int32*>(data));
		case Config::DataType::Float: return static_cast<bool>(*reinterpret_cast<float*>(data));
		case Config::DataType::Boolean: return *reinterpret_cast<bool*>(data);
		case Config::DataType::String: return !reinterpret_cast<std::string*>(data)->empty();
	}
	return default_value;
}
const std::string& Config::Data::getString(const std::string& default_value) const
{
	switch (type)
	{
		case Config::DataType::Null: return default_value;
		case Config::DataType::Integer: return std::move(std::to_string(*reinterpret_cast<int32*>(data)));
		case Config::DataType::Float: return std::move(std::to_string(*reinterpret_cast<float*>(data)));
		case Config::DataType::Boolean: return std::move(std::to_string(*reinterpret_cast<bool*>(data)));
		case Config::DataType::String: reinterpret_cast<std::string*>(data);
	}
	return default_value;
}



Config Config::_instance{};

Config::Config() :
	_map{}
{}

void Config::setNull(const std::string& key) { _InsertValue(key, nullptr, &Data::setNull); }
void Config::setInteger(const std::string& key, int32 value) { _InsertValue(key, value, &Data::setInteger); }
void Config::setFloat(const std::string& key, float value) { _InsertValue(key, value, &Data::setFloat); }
void Config::setBoolean(const std::string& key, bool value) { _InsertValue(key, value, &Data::setBoolean); }
void Config::setString(const std::string& key, const std::string& value) { _InsertValue(key, value, &Data::setString); }

bool Config::isNull(const std::string& key)
{
	const auto& it = Config::_instance._map.find(key);
	return it == Config::_instance._map.end() || it->second.isNull();
}
int32 Config::getInteger(const std::string& key, int32 default_value)
{
	const auto& it = Config::_instance._map.find(key);
	return it == Config::_instance._map.end() ? default_value : it->second.getInteger(default_value);
}
float Config::getFloat(const std::string& key, float default_value)
{
	const auto& it = Config::_instance._map.find(key);
	return it == Config::_instance._map.end() ? default_value : it->second.getFloat(default_value);
}
bool Config::getBoolean(const std::string& key, bool default_value)
{
	const auto& it = Config::_instance._map.find(key);
	return it == Config::_instance._map.end() ? default_value : it->second.getBoolean(default_value);
}
const std::string& Config::getString(const std::string& key, const std::string& default_value)
{
	const auto& it = Config::_instance._map.find(key);
	return it == Config::_instance._map.end() ? default_value : it->second.getString(default_value);
}


Config& __ConfigInstance() { return Config::_instance; }
