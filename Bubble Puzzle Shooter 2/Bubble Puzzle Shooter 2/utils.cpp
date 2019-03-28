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