#ifndef COVUSCONTROL_H
#define COVUSCONTROL_H
#include "config/Config_t.h"
#include "Socket/TCPConnector.h"
#include "Socket/TCPStream.h"
#include "container/updateinfo.h"
#include "container/updatecollection.h"
#include <memory>



class CovusControl
{
public:
    enum class State_t
    {
      IDLE,CONFIGURED,BUSSY,ERROR
    };

    CovusControl();
    void Configure(Config_t* const rConfig);
    void Execute(const Pos_t &SinglePos);
    void Stop();
    void Reset();
    CovusControl::State_t GetState(Corvuspos& r_CorvPos);
    void Finish();


    inline void SetStateIdle()
    {
        this->m_state_t = State_t::IDLE;
    }

private:
    State_t m_state_t;
    std::unique_ptr<TCPStream> m_TCPConCorv;
    unsigned int m_CorvCtrlTimeout;

};

#endif // COVUSCONTROL_H
