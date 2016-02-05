#ifndef LTSERROR_H
#define LTSERROR_H

#include <string>

namespace LTS {

class LTSError
{
	public:
		enum class Error_t
		{
			WARNING,
			FATAL
        };

		LTSError(const Error_t Type, const std::string& rWho,
						 const std::string& rWhat) noexcept;

		std::string What() const noexcept;

        inline const std::string& GetWho() const noexcept
		{
			return m_Who;
		}

        inline const std::string& GetWhat() const noexcept
		{
			return m_What;
		}

        inline const Error_t& GetType() const noexcept
		{
			return m_Type;
		}

	private:
		const Error_t m_Type;
		const std::string m_Who;
		const std::string m_What;
};

}

#endif // LTSERROR_H
