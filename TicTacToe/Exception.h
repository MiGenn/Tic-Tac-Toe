#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(int line, const char* file) noexcept;

	virtual const char* what() const noexcept override final;
	virtual const std::string& GetDescription() const noexcept = 0;

protected:
	const std::string& GetOrigin() const noexcept;

private:
	std::string m_origin;
};
