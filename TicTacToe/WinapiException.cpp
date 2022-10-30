#include "WinapiException.h"
#include "WinapiUtilities.h"
#include <sstream>

WinapiException::WinapiException(int line, const char* file, HRESULT errorCode) noexcept :
	Exception(line, file)
{
	std::stringstream ss;
	ss << "ERROR CODE: " << std::hex << errorCode << '\n' <<
		"DESCRIPTION: " << GetErrorDescription(errorCode) << '\n' << GetOrigin();
	m_description = ss.str();
}

const std::string& WinapiException::GetDescription() const noexcept
{
	return m_description;
}
