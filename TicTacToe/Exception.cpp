#include "Exception.h"
#include <sstream>

Exception::Exception(int line, const char* file) noexcept
{
	std::stringstream ss;
	ss << "FILE: " << file << '\n' << "LINE: " << line;
	m_origin = ss.str();
}

const char* Exception::what() const noexcept
{
	return GetDescription().c_str();
}

const std::string& Exception::GetOrigin() const noexcept
{
	return m_origin;
}
