#include "utils.h"

uuid uuid::generate()
{
	static uint64 __generator = 0;
	return uuid(++__generator);
}


Throwable::Throwable(const std::string msg, const Throwable* cause) : message(msg), cause(cause) {}

Throwable::~Throwable()
{
	cause = nullptr;
}

BPSException::BPSException(const std::string msg, const Throwable* cause) : Throwable(msg, cause) {}




void  LocalAttrAllocator::setLocalInt(const std::string& name, const int32& value)
{
	setLocal(_ints, name, value);
}

int32 LocalAttrAllocator::getLocalInt(const std::string& name, const int32& default_value)
{
	return getLocal(_ints, name, default_value);
}

bool  LocalAttrAllocator::deleteLocalInt(const std::string& name)
{
	return deleteLocal(_ints, name);
}

void  LocalAttrAllocator::setLocalFloat(const std::string& name, const float& value)
{
	setLocal(_flts, name, value);
}

float LocalAttrAllocator::getLocalFloat(const std::string& name, const float& default_value)
{
	return getLocal(_flts, name, default_value);
}

bool  LocalAttrAllocator::deleteLocalFloat(const std::string& name)
{
	return deleteLocal(_flts, name);
}

void  LocalAttrAllocator::setLocalString(const std::string& name, const std::string& value)
{
	setLocal(_strs, name, value);
}

std::string LocalAttrAllocator::getLocalString(const std::string& name, const std::string& default_value)
{
	return getLocal(_strs, name, default_value);
}

bool  LocalAttrAllocator::deleteLocalString(const std::string& name)
{
	return deleteLocal(_strs, name);
}

void LocalAttrAllocator::deleteAllLocals()
{
	_ints.clear();
	_flts.clear();
	_strs.clear();
}











degrees_t::degrees_t(const radians_t& rad) : _value(RAD_2_DEG(rad)) {}

degrees_t& degrees_t::operator= (const float& v2)
{
	_value = fix(v2);
	return *this;
}

degrees_t& degrees_t::operator= (const radians_t& rad)
{
	_value = RAD_2_DEG(rad._value);
	return *this;
}

degrees_t::operator float() const
{
	return _value;
}

degrees_t& degrees_t::operator+= (const degrees_t& v2) { return *this = *this + v2; }
degrees_t& degrees_t::operator+= (const radians_t& v2) { return *this = *this + v2; }
degrees_t& degrees_t::operator+= (const float& v2) { return *this = *this + v2; }

degrees_t& degrees_t::operator-= (const degrees_t& v2) { return *this = *this - v2; }
degrees_t& degrees_t::operator-= (const radians_t& v2) { return *this = *this - v2; }
degrees_t& degrees_t::operator-= (const float& v2) { return *this = *this - v2; }

degrees_t& degrees_t::operator*= (const degrees_t& v2) { return *this = *this * v2; }
degrees_t& degrees_t::operator*= (const radians_t& v2) { return *this = *this * v2; }
degrees_t& degrees_t::operator*= (const float& v2) { return *this = *this * v2; }

degrees_t& degrees_t::operator/= (const degrees_t& v2) { return *this = *this / v2; }
degrees_t& degrees_t::operator/= (const radians_t& v2) { return *this = *this / v2; }
degrees_t& degrees_t::operator/= (const float& v2) { return *this = *this / v2; }





radians_t::radians_t(const degrees_t& deg) : _value(DEG_2_RAD(deg)) {}

radians_t& radians_t::operator= (const float& v2)
{
	_value = v2;
	return *this;
}

radians_t& radians_t::operator= (const degrees_t& deg)
{
	_value = DEG_2_RAD(deg._value);
	return *this;
}

radians_t::operator float() const
{
	return _value;
}

radians_t& radians_t::operator+= (const radians_t& v2) { return *this = *this + v2; }
radians_t& radians_t::operator+= (const degrees_t& v2) { return *this = *this + v2; }
radians_t& radians_t::operator+= (const float& v2) { return *this = *this + v2; }

radians_t& radians_t::operator-= (const radians_t& v2) { return *this = *this - v2; }
radians_t& radians_t::operator-= (const degrees_t& v2) { return *this = *this - v2; }
radians_t& radians_t::operator-= (const float& v2) { return *this = *this - v2; }

radians_t& radians_t::operator*= (const radians_t& v2) { return *this = *this * v2; }
radians_t& radians_t::operator*= (const degrees_t& v2) { return *this = *this * v2; }
radians_t& radians_t::operator*= (const float& v2) { return *this = *this * v2; }

radians_t& radians_t::operator/= (const radians_t& v2) { return *this = *this / v2; }
radians_t& radians_t::operator/= (const degrees_t& v2) { return *this = *this / v2; }
radians_t& radians_t::operator/= (const float& v2) { return *this = *this / v2; }
