#ifndef LTSCORE_H
#define LTSCORE_H
#include "LTSctrl.h"
#include "Corvus/covuscontrol.h"
#include "Tuxdaqctrl/LTS_DAQCtrl.h"
#include "dummytux.h"
#include "config/Config_t.h"
#include <thread>
#include <mutex>
#include "container/updateinfo.h"
#include "log/Logger.h"

class LTSCore
{
public:
    enum class Transition_t
    {
        IDLE,
        CONFIGURE,
        START,
        STOP,
        CONTINUE,
        RESET,
        EXIT,
        SET

    };

public:
    LTSCore(std::shared_ptr<updateinfo> sptr_updateinfo);
    ~LTSCore();
    void RunCore();
        void CoreThread();

        inline void SetStateReq(Transition_t RequestedState) noexcept
        {

            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_ReqTransState = RequestedState;

        }

        inline Transition_t GetStateReq() noexcept
        {

            std::lock_guard<std::mutex> lock(m_mutex);
            return this->m_ReqTransState;
        }

        inline void SetConfigCore(Config_t ConfigCore) noexcept
        {
            if(m_uptr_ConfigCore!=nullptr)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_uptr_ConfigCore.reset(nullptr);
            }

            std::lock_guard<std::mutex> lock(m_mutex);
            m_uptr_ConfigCore = std::make_unique<Config_t>(ConfigCore);
        }

    //    inline void Set_smart_ptrCorvusCore(std::shared_ptr<updateinfo> vsp) noexcept
    //    {
    //        this->m_sptr_CorvusCore = vsp;
    //    }





private:
        //std::unique_ptr<LTS::LTSctrl<CovusControl,dummytux>> m_uptr_LTSFSM;
        std::unique_ptr<LTS::LTSctrl<CovusControl,LTS::LTS_DAQCtrl>> m_uptr_LTSFSM;
        std::unique_ptr<std::thread> m_uptr_FSMThread;
        std::unique_ptr<Config_t> m_uptr_ConfigCore;

        std::shared_ptr<updateinfo> m_sptr_CorvusCore;

        Transition_t m_ReqTransState;
        std::mutex m_mutex;

};

#endif // LTSCORE_H
