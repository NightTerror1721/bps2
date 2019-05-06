#include "config.h"

u64 UUID::__gen;

UUID::UUID(u64 code) :
	_code(code)
{}

UUID UUID::generate()
{
	return { ++__gen };
}

bool UUID::operator== (const UUID& uuid) const { return _code == uuid._code; }
bool UUID::operator!= (const UUID& uuid) const { return _code != uuid._code; }
bool UUID::operator> (const UUID& uuid) const { return _code > uuid._code; }
bool UUID::operator< (const UUID& uuid) const { return _code < uuid._code; }
bool UUID::operator>= (const UUID& uuid) const { return _code >= uuid._code; }
bool UUID::operator<= (const UUID& uuid) const { return _code <= uuid._code; }

std::ostream& operator<< (std::ostream& os, const UUID& uuid) { return os << uuid._code; }
std::istream& operator>> (std::istream& is, UUID& uuid) { return is >> uuid._code; }



UniqueObject::UniqueObject() :
	_id(UUID::generate())
{}

const UUID& UniqueObject::uuid() const { return _id; }

bool UniqueObject::operator== (const UniqueObject& uo) { return _id == uo._id; }
bool UniqueObject::operator!= (const UniqueObject& uo) { return _id != uo._id; }

