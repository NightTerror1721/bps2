#include "config.h"

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
