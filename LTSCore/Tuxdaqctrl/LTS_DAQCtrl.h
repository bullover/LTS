#ifndef LTS_DAQCTRL_H
#define LTS_DAQCTRL_H

#include <memory>
#include "config/Config_t.h"
#include "../Socket/TCPStream.h"
#include "container/updateinfo.h"
namespace LTS {

class LTS_DAQCtrl
{
private:
	// in units of secs
	static constexpr unsigned int m_gTIMEOUT = 1000;

public:
	enum class State_t : unsigned char
	{
		IDLE = 0,
		READY = 1,
		BUSSY = 2,
		ERROR = 0xff
	};

//	LTS_DAQCtrl() noexcept
//		: m_NrUpdate(0), m_EventCnt(0), m_Port(0), m_IP()
//	{}

    LTS_DAQCtrl(std::shared_ptr<updateinfo> sptr_updateinfo) noexcept;

	// disbale copy and copy assignmnet
	LTS_DAQCtrl(const LTS_DAQCtrl&) = delete;
	LTS_DAQCtrl operator=(const LTS_DAQCtrl&) = delete;

	// basic default move ctor with no except
	LTS_DAQCtrl(LTS_DAQCtrl&& rOther) noexcept;

	// basic default move assignment ctor with no except
	LTS_DAQCtrl& operator=(LTS_DAQCtrl&& rOther) noexcept;

	// configure with defined config structure
    void Configure(Config_t* const rConfig);

	// take next few events
	void Execute();

	// finish daq
	void Finish();

	// get state
    State_t GetState() noexcept;

    inline void set_smart_ptrDAQCtrl(std::shared_ptr<updateinfo> spValue)
    {
        this->sp_DAQCntrl = spValue;

    }

private:
	std::unique_ptr<TCPStream> m_Socket;
	TuxDAQConfig_t m_Config;
	State_t m_State;
    std::shared_ptr<updateinfo> sp_DAQCntrl;
};

}

#endif // LTS_DAQCTRL_H
