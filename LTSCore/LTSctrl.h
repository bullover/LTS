#ifndef LTSCTRL_H
#define LTSCTRL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "config/Config_t.h"
#include <thread>
#include <mutex>
#include <chrono>
#include "Error/LTSError.h"
#include "bin/tobin.h"
#include "bin/frombin.h"
#include "container/updateinfo.h"
#include "log/Logger.h"
#include <sstream>
//#include "container/updatecollection.h"




namespace LTS {

template <class XYTable, class MeasureSys>
class LTSctrl
{
public:

    enum class State_t
    {
        IDLE = 0,CONFIGURED = 1, RUNNING = 2, STOPPED = 3, ERROR = 4, BUSSY = 5

    };

    inline State_t GetLTSState()
    {
        // std::lock_guard<std::mutex> lock(m_LTSmutex);
        return this->m_State;
    }


    LTSctrl(std::shared_ptr<updateinfo> sptr_updateinfo)
        : m_XYTable(), m_MeasureSys(sptr_updateinfo), m_State(State_t::IDLE), m_XYPos(),
          m_BinFileName("/home/lts/Documents/TUX/LTSv6/LTSCore/bin/path.bin"),m_sptr_LTSCtrl(sptr_updateinfo)
    {

    }
///-----------------------------Conf ----------------------------------------------------------------///
    void Configure( Config_t* const rConfig) noexcept
    {
        if(m_State != State_t::IDLE && m_State != State_t::ERROR&& m_State != State_t::STOPPED)
        {
            std::stringstream ss;
            ss << "cant configure if its not idle" ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());

            return void();
        }

        if(m_sptr_LTSCtrl == nullptr)
        {
            std::stringstream ss;
            ss << "smart ptr to update info container is not set" ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());

            return void();
        }

        std::stringstream ss;
        ss << "LTScrtl configure is runnning\n" ;
        GlobalLogger::Log(Logger::Type_t::INFO, "CorvusCore",
                          ss.str());



        try{

            Corvuspos t_CorvPos;
            //should be only configured if there is no connection
            if(m_State==State_t::IDLE || m_MeasureSys.GetState()==MeasureSys::State_t::ERROR)
            {
                m_MeasureSys.Configure(rConfig);
            }

            while(m_MeasureSys.GetState()==MeasureSys::State_t::BUSSY) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }


            m_XYTable.Configure(rConfig);
            while(m_XYTable.GetState(t_CorvPos)==XYTable::State_t::BUSSY) {
                m_sptr_LTSCtrl->SetPos(t_CorvPos);
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }


            m_XYPos.clear();
            m_XYPos.resize(rConfig->m_Corvus.m_allPosabsolut.size());
//            std::copy(rConfig.m_Corvus.m_allPosabsolut.begin(),
//                      rConfig.m_Corvus.m_allPosabsolut.end(),
//                      m_XYPos.begin());

            std::reverse_copy(rConfig->m_Corvus.m_allPosabsolut.begin(),rConfig->m_Corvus.m_allPosabsolut.end(),
                              m_XYPos.begin());

            m_State = State_t::CONFIGURED;
            m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::CONFIGURED);

        }catch(const LTSError& rError)
        {
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                                rError.What());
          m_State = State_t::ERROR;
          m_XYTable.SetStateIdle();
          m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::ERROR);


        }


    }
///-----------------------------Conf END-------------------------------------------------------------///
///-----------------------------START----------------------------------------------------------------///
    void Start() noexcept
    {
        if(m_State != State_t::CONFIGURED && m_State != State_t::RUNNING )
        {
            std::stringstream ss;
            ss << "cant start if its not configured" ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());
            return void();

        }


        m_State = State_t::RUNNING;

        m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::RUNNING);


        try
        {
            Corvuspos t_CorvPos;

            if(!m_XYPos.empty())
            {
                m_XYTable.Execute(m_XYPos.back());
                m_XYPos.pop_back();
                while(m_XYTable.GetState(t_CorvPos)==XYTable::State_t::BUSSY) {
                    m_sptr_LTSCtrl->SetPos(t_CorvPos);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
                m_MeasureSys.Execute();
                while(m_MeasureSys.GetState()==MeasureSys::State_t::BUSSY) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }


            }else
            {
                this->Finished();
            }

//            for(Pos_t a : m_XYPos)
//            {
//            //std::cout<<"start: " << a.m_XPos<< " & "<< a.m_YPos << std::endl;
//               }
        }catch(const LTSError& rError)
        {
          {
                ToBin wFile(m_BinFileName);
                for(Pos_t a : m_XYPos)
                {
                    wFile.writeBin(a);
                }
                wFile.closeStream();
          }
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                                rError.What());
          m_State = State_t::ERROR;
          m_XYTable.SetStateIdle();
          m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::ERROR);

        }


    }
    ///-----------------------------START END----------------------------------------------------------------///

    ///-----------------------------Finished ----------------------------------------------------------------///
    void Finished()noexcept
    {
        if(m_State != State_t::RUNNING)
        {
            std::stringstream ss;
            ss << "cant finish if its not started " ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());
            return void();

        }

        try
        {
            Corvuspos t_CorvPos;
            m_MeasureSys.Finish();
            while(m_MeasureSys.GetState()==MeasureSys::State_t::BUSSY) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            m_XYTable.Finish();


            while(m_XYTable.GetState(t_CorvPos)==XYTable::State_t::BUSSY) {
                m_sptr_LTSCtrl->SetPos(t_CorvPos);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }


            m_State = State_t::IDLE;
            m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::IDLE);

        }catch(const LTSError& rError)
        {
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                                rError.What());
          m_State = State_t::ERROR;
          m_XYTable.SetStateIdle();
          m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::ERROR);

        }

    }
    ///-----------------------------Finished END----------------------------------------------------------------///

    ///-----------------------------STOP----------------------------------------------------------------///
    void Stop() noexcept
    {

       if(m_State != State_t::BUSSY && m_State != State_t::RUNNING)
        {
            std::stringstream ss;
            ss << "cant stop wrong state " ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());
            return void();

        }
       m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::BUSSY);

        try{
           // m_XYTable.Stop();
            //fehlt noch tuxdaq stop und was passiert wenn in while schleife von start()
            {
                ToBin wFile(m_BinFileName);
                for(Pos_t a : m_XYPos)
                {
                    wFile.writeBin(a);
                }
                wFile.closeStream();
            }
            m_State = State_t::STOPPED;
            m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::STOPPED);

        }catch(const LTSError& rError)
        {
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                                rError.What());
          m_State = State_t::ERROR;
          m_XYTable.SetStateIdle();
          m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::ERROR);

        }
    }
    ///-----------------------------STOP END----------------------------------------------------------------///

    ///-----------------------------RESET----------------------------------------------------------------///
    void Reset() noexcept
    {
        if(m_State != State_t::STOPPED)
        {
            std::stringstream ss;
            ss <<"cant reset in the currrent state" ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());
            return void();

        }

        try{
             m_XYPos.clear();
             m_XYTable.Reset();
             m_State = State_t::IDLE;
             m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::IDLE);

        }catch(const LTSError& rError)
        {
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                                rError.What());
          m_State = State_t::ERROR;
          m_XYTable.SetStateIdle();
          m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::ERROR);

        }
    }
///-----------------------------RESET END----------------------------------------------------------------///

///-----------------------------CONTINUE ----------------------------------------------------------------///
    void Continue()noexcept
    {
        if(m_State != State_t::STOPPED  &&  m_State != State_t::CONFIGURED )
        {
            std::stringstream ss;
            ss <<"cant continue in the currrent state " ;
            GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              ss.str());


            return void();

        }
        //m_sptr_LTSCtrl->SetSubState(updateinfo::Substate_t::OCCUPIED);
        try{

             m_XYPos.clear();

             {
                   std::vector<Pos_t> l_vecbuff;
                   FromBin rFile(m_BinFileName);
                   Pos_t a;
                   while(rFile.readBin(a))
                   {
                       l_vecbuff.push_back(a);

                   }
                   rFile.closeStream();
                   m_XYPos.resize(l_vecbuff.size());
                   std::copy(l_vecbuff.begin(),l_vecbuff.end(),m_XYPos.begin());
             }

             m_State = State_t::CONFIGURED;
             m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::CONFIGURED);
           //  m_sptr_LTSCtrl->SetSubState(updateinfo::Substate_t::READY);

//             for(Pos_t a : m_XYPos)
//             {
//             std::cout<< "cont: " << a.m_XPos<< " & "<< a.m_YPos << std::endl;
//                }
        }catch(const LTSError& rError)
        {

          GlobalLogger::Log(Logger::Type_t::WARNING, "LTSctrl",
                              rError.What());

          m_State = State_t::ERROR;
          m_XYTable.SetStateIdle();
          m_sptr_LTSCtrl->SetLTSState(updateinfo::State_t_LTSCtrl::ERROR);
         // m_sptr_LTSCtrl->SetSubState(updateinfo::Substate_t::READY);
        }

    }
///-----------------------------CONTINUE END----------------------------------------------------------------///
private:
   // std::unique_ptr<XYTable>m_XYTable;
    XYTable m_XYTable;
    MeasureSys m_MeasureSys;
    State_t m_State;
    std::vector<Pos_t> m_XYPos;
    std::string m_BinFileName;
    std::shared_ptr<updateinfo> m_sptr_LTSCtrl;
  //  std::mutex m_LTSmutex;


};

}

#endif // LTSCTRL_H
