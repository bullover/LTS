#include "ltscore.h"
#include <sstream>


LTSCore::LTSCore(std::shared_ptr<updateinfo> sptr_updateinfo): m_uptr_LTSFSM(nullptr),m_uptr_FSMThread(nullptr),m_uptr_ConfigCore(nullptr),
    m_sptr_CorvusCore(sptr_updateinfo),m_ReqTransState(Transition_t::IDLE),m_mutex()
{
    this->m_uptr_LTSFSM = std::make_unique<LTS::LTSctrl<CovusControl,LTS::LTS_DAQCtrl>>(this->m_sptr_CorvusCore);
    //this->m_uptr_LTSFSM = std::make_unique<LTS::LTSctrl<CovusControl,dummytux>>(this->m_sptr_CorvusCore);
    m_uptr_FSMThread = std::make_unique<std::thread>(&LTSCore::CoreThread,this);
   // this->m_uptr_LTSFSM->Set_smart_ptr(this->m_sptr_CorvusCore);


}

LTSCore::~LTSCore()
{
    if (this->m_uptr_FSMThread->joinable()) m_uptr_FSMThread->join();
}

void LTSCore::RunCore()
{


//    if(m_uptr_ConfigCore==nullptr)!!!!!
//    {
//        std::stringstream ss;
//        ss << "m_sptr_CorvusCore is nullptr" ;

//        GlobalLogger::Log(Logger::Type_t::WARNING, "CorvusCore",
//                          ss.str());
//        return void();
  //  }

    if(m_sptr_CorvusCore==nullptr)
    {
//        std::stringstream ss;
//        ss << "m_sptr_CorvusCore is nullptr" ;

//        GlobalLogger::Log(Logger::Type_t::WARNING, "CorvusCore",
//                          ss.str());
        return void();
    }

    switch (this->GetStateReq())
    {
        case LTSCore::Transition_t::CONFIGURE:
             this->m_uptr_LTSFSM->Configure(this->m_uptr_ConfigCore.get());
             SetStateReq(LTSCore::Transition_t::SET);
             GlobalLogger::Log(Logger::Type_t::INFO,"CORVUSCORE:","CONFIGURED");
            break;
        case LTSCore::Transition_t::START:
             this->m_uptr_LTSFSM->Start();
             GlobalLogger::Log(Logger::Type_t::INFO,"CORVUSCORE:","RUNNING");
             //SetStateReq(CorvusCore::Transition_t::SET);
            break;
        case LTSCore::Transition_t::STOP:
             this->m_uptr_LTSFSM->Stop();
             SetStateReq(LTSCore::Transition_t::SET);
             GlobalLogger::Log(Logger::Type_t::INFO,"CORVUSCORE:","STOPED");
            break;
        case LTSCore::Transition_t::CONTINUE:
             this->m_uptr_LTSFSM->Continue();
             GlobalLogger::Log(Logger::Type_t::INFO,"CORVUSCORE:","CONTINUED");
             this->SetStateReq(Transition_t::START);
            break;
        case LTSCore::Transition_t::RESET:
            this->m_uptr_LTSFSM->Reset();
            SetStateReq(LTSCore::Transition_t::SET);
            GlobalLogger::Log(Logger::Type_t::INFO,"CORVUSCORE:","RESETED");
            break;
        default:
            break;
    }

//    if(GetStateReq()!= CorvusCore::Transition_t::START )
//        this->SetStateReq(Transition_t::IDLE);

//    if( m_uptr_LTSFSM->GetLTSState() == m_uptr_LTSFSM->State_t::IDLE)
//            this->SetStateReq(Transition_t::IDLE);

}

void LTSCore::CoreThread()
{
    while(this->GetStateReq() != LTSCore::Transition_t::EXIT)
    {
        this->RunCore();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

}
