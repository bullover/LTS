#include "Logger.h"

#include <ctime>
#include <sstream>
#include <iostream>

Logger::Logger() noexcept
	: fLoggers()
{
    //if (size) (this->fLoggers).reserve(size);
    (this->fLoggers).reserve(5);
}

Logger& Logger::operator=(Logger&& rOther) noexcept
{
	(this->fLoggers) = std::move(rOther.fLoggers);
	return *this;
}

Logger::Logger(Logger&& rOther) noexcept
	: fLoggers(std::move(rOther.fLoggers))
{}

void Logger::Log(Logger::Type_t type, const std::string& rWho,
								 const std::string& rWhat) noexcept
{
	if ((this->fLoggers).empty()) return void();

	std::stringstream ss;
	// set time stamp
	{
		time_t rawTime;
		struct tm * tInfo;
		char curTime[25];

		time (&rawTime);
		tInfo = localtime (&rawTime);

		strftime(curTime,25, "%Y:%d:%m %T::",tInfo);
		ss << curTime;
	}

	// set thrower
	ss << rWho << "::";
	switch (type) {
		case Logger::Type_t::WARNING:
			ss << "WARNING::";
			break;
		case Logger::Type_t::FATAL:
			ss << "FATAL ERROR::";
			break;
		case Logger::Type_t::INFO:
			ss << "INFO::";
			break;
		default:
			ss << "UNKNOWN::";
			break;
	}
	ss << rWhat << '\n';
	const auto msg = ss.str();

	// write msg
	for(auto& rLogger : this->fLoggers) {
		rLogger->Write(msg);
	}
}

std::unique_ptr<GlobalLogger> GlobalLogger::fgpLogger(nullptr);

