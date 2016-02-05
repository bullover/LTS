#include "LTSError.h"
#include <sstream>

namespace LTS {

LTSError::LTSError(const LTSError::Error_t Type,
									 const std::string& rWho,
									 const std::string& rWhat) noexcept
	: m_Type(Type), m_Who(rWho), m_What(rWhat)
{}

std::string LTSError::What() const noexcept
{
	std::stringstream ss;
	ss << m_Who << "::"
		 << (Error_t::WARNING == m_Type ? "WARNING::" : "ERROR::")
		 << m_What;
	return ss.str();
}

}
