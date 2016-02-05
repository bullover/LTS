#ifndef UPDATEINFO_H
#define UPDATEINFO_H
#include <string>
#include <thread>
#include <mutex>
#include "updatecollection.h"


class updateinfo
{
public:

    enum class State_t_LTSCtrl
    {
        IDLE = 0,CONFIGURED = 1, RUNNING = 2, STOPPED = 3, ERROR = 4, BUSSY = 5

    };


    updateinfo();

    inline Corvuspos GetPos()
    {
        std::lock_guard<std::mutex> lock(m_UpdateMtx);
        return this->m_Pos;
    }

    inline void SetLTSState(State_t_LTSCtrl state)
    {
        std::lock_guard<std::mutex> lock(m_UpdateMtx);
        this->m_StateLTSCtrl = state;
    }

    inline State_t_LTSCtrl GetLTSState()
    {
        std::lock_guard<std::mutex> lock(m_UpdateMtx);
        return this->m_StateLTSCtrl;
    }

    inline void SetPos(Corvuspos Pos)
    {
        std::lock_guard<std::mutex> lock(m_UpdateMtx);
        this->m_Pos = Pos;
    }


private:
    Corvuspos m_Pos;
    State_t_LTSCtrl m_StateLTSCtrl;
    std::mutex m_UpdateMtx;


};

#endif // UPDATEINFO_H
