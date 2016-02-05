#include "LTS_DAQCtrl.h"
#include "../Socket/TCPConnector.h"

#include "TuxDAQCMD.h"
#include "../Error/LTSError.h"

#include <sstream>
#include <iostream>

#include <chrono>
#include <thread>



namespace LTS {

LTS_DAQCtrl::LTS_DAQCtrl(std::shared_ptr<updateinfo> sptr_updateinfo) noexcept
    : m_Socket(nullptr), m_Config(), m_State(LTS_DAQCtrl::State_t::IDLE),sp_DAQCntrl(sptr_updateinfo)
{}

LTS_DAQCtrl::LTS_DAQCtrl(LTS_DAQCtrl&& rOther) noexcept
	: m_Socket(std::move(rOther.m_Socket)),
		m_Config(std::move(rOther.m_Config)),
        m_State(rOther.m_State),sp_DAQCntrl(nullptr)
{}

LTS_DAQCtrl& LTS_DAQCtrl::operator=(LTS_DAQCtrl&& rOther) noexcept
{
	m_Socket = std::move(rOther.m_Socket);
	m_Config = std::move(rOther.m_Config);
	m_State = rOther.m_State;

	return *this;
}

void LTS_DAQCtrl::Configure(Config_t * const rConfig)
{
    if (m_State != LTS_DAQCtrl::State_t::IDLE && m_State != LTS_DAQCtrl::State_t::ERROR) {
		std::stringstream ss;
		ss << "Can not perform Configure(...), if the system is not IDLE.";
		throw LTSError(LTSError::Error_t::WARNING, "LTS_DAQCtrl", ss.str());
	}
    if(sp_DAQCntrl == nullptr)
    {
        std::stringstream ss;
        ss << "Can not perform Configure(...), if updatecontainer is not allocated.";
        throw LTSError(LTSError::Error_t::WARNING, "LTS_DAQCtrl", ss.str());

    }
	m_State = LTS_DAQCtrl::State_t::BUSSY;
    m_Config = rConfig->m_TuxDAQ;
	TCPConnector conn;
	m_Socket.reset(nullptr);

	for (unsigned int i = 1000; (i--) && (m_Socket.get() == nullptr);) {
		m_Socket.reset(conn.Connect(m_Config.m_Port, m_Config.m_IP));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (m_Socket.get() == nullptr) {
		m_State = LTS_DAQCtrl::State_t::ERROR;
		std::stringstream ss;
		ss << "Can not connect to TuxDAQ with the given port: '" << m_Config.m_Port
			 << "'' and IP: '" << m_Config.m_IP << "'.";
		throw LTSError(LTSError::Error_t::FATAL, "LTS_DAQCtrl", ss.str());
	}

	m_State = LTS_DAQCtrl::State_t::READY;
}

void LTS_DAQCtrl::Execute()
{
	if (LTS_DAQCtrl::State_t::READY != m_State) {
		std::stringstream ss;
		ss << "Can not perform Execute(...), if the system is not READY.";
		throw LTSError(LTSError::Error_t::WARNING, "LTS_DAQCtrl", ss.str());
	}

	auto cmd = uint32_t(TuxDAQCMD::gEXCECUTE | (m_Config.m_NrUpdate & 0xffffff));
	auto nrSent = m_Socket->SendWord(cmd);
	if (sizeof(cmd) != nrSent) {
		m_State =LTS_DAQCtrl::State_t::ERROR;
		std::stringstream ss;
		ss << "Connection was successfull. Can not send execute word. Expected "
			 << sizeof(cmd) << " got " << nrSent << ".";
		throw LTSError(LTSError::Error_t::FATAL, "LTS_DAQCtrl", ss.str());
	}

	m_State = LTS_DAQCtrl::State_t::BUSSY;
}

void LTS_DAQCtrl::Finish()
{
	if (LTS_DAQCtrl::State_t::READY != m_State) {
		std::stringstream ss;
		ss << "Can not perform Execute(...), if the system is not READY.";
		throw LTSError(LTSError::Error_t::WARNING, "LTS_DAQCtrl", ss.str());
		return void();
	}

	auto nrSent = m_Socket->SendWord(TuxDAQCMD::gFINISH);
	if (sizeof(LTS::TuxDAQCMD::gFINISH) != nrSent) {
		m_State = LTS_DAQCtrl::State_t::ERROR;
		std::stringstream ss;
		ss << "Connection was successfull. Can not send finish word. Expected "
			 << sizeof(LTS::TuxDAQCMD::gFINISH) << " got " << nrSent << ".";
		throw LTSError(LTSError::Error_t::FATAL, "LTS_DAQCtrl", ss.str());
		return void();
	}

	m_State = LTS_DAQCtrl::State_t::IDLE;
}

LTS_DAQCtrl::State_t LTS_DAQCtrl::GetState() noexcept
{
	if ((m_State == LTS_DAQCtrl::State_t::BUSSY) &&
			(m_Socket.get() != nullptr))
	{
		auto word = uint32_t{0u};
		auto retval = m_Socket->ReceiveWord(word, LTS_DAQCtrl::m_gTIMEOUT);
		if (retval > 0) {
			std::cout << "Statew: " << word << std::endl;
			switch (word & 0xff000000) {
				case TuxDAQCMD::gPERFORMED:
				{
					m_State = LTS_DAQCtrl::State_t::READY;
					break;
				}
				case TuxDAQCMD::gERROR:
				{
					m_State = LTS_DAQCtrl::State_t::ERROR;
					break;
				}
			}

			m_State = (word != TuxDAQCMD::gPERFORMED) ?
						LTS_DAQCtrl::State_t::BUSSY : LTS_DAQCtrl::State_t::READY;
			m_State = (word != TuxDAQCMD::gERROR) ?
						m_State : LTS_DAQCtrl::State_t::ERROR;
        } else if ((0 >= retval) && (-3 != retval)) {
			m_State = LTS_DAQCtrl::State_t::ERROR;
            throw LTSError(LTSError::Error_t::FATAL, "Tuxdaq", "connection lost.");
		}
	}


	return m_State;
}

}
