#include "covuscontrol.h"
#include "Error/LTSError.h"
#include <thread>
#include <chrono>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "log/Logger.h"


CovusControl::CovusControl():m_state_t(State_t::IDLE),m_TCPConCorv(nullptr),
                        m_CorvCtrlTimeout(0)

{}

void CovusControl::Configure(Config_t* const rConfig)
{
    if(m_state_t == State_t::BUSSY )
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","state is not idle");
        return void();
    }
//    if(sp_CorvusCtrl == nullptr)
//    {
//        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","no update container allocated");
//        return void();
//    }

        m_state_t = State_t::BUSSY;
        m_CorvCtrlTimeout = rConfig->m_Corvus.m_TimeOutCorv;


        std::string addr(rConfig->m_Corvus.m_IPCorv) ;
//        if(m_TCPConCorv == nullptr)
//        {
            auto connector = TCPConnector();
            m_TCPConCorv.reset(connector.Connect(rConfig->m_Corvus.m_PortCorv,addr));
//        }

        if(m_TCPConCorv == nullptr)
        {
           std::cout << "is not connected" << "\n";
           throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","no connection to Corvus");
        }else
        {

//           std::cout << "is connected to client" << "\n";
           for(auto ConfString : rConfig->m_Corvus.m_Configsetup)
           {
               m_TCPConCorv->Send( ConfString  + char(0xd));
               std::this_thread::sleep_for(std::chrono::milliseconds(200));
           }

            m_state_t = State_t::CONFIGURED;
            this->Reset();
        }
}


void CovusControl::Execute(const Pos_t &SinglePos )
{

    if(m_state_t != State_t::CONFIGURED)
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","state is not configured");
        return void();
    }

    m_state_t = State_t::BUSSY;
    std::stringstream l_ExecuteStr;
    //std::string GetStateStr;
    //<< std::setprecision(2)<< std::fixed
    l_ExecuteStr << SinglePos.m_XPos<<" "<<SinglePos.m_YPos<<" "<<"m"<<char(0xd);
   // std::cout << l_ExecuteStr.str() << "-send execute string\n" << std::endl;
    m_TCPConCorv->Send(l_ExecuteStr.str() );
    //std::cout << m_TCPConCorv->Receive(GetStateStr,m_CorvCtrlTimeout,char(0xd)) << " -concorvus " << std::endl;
    //std::cout << "position  x: " << SinglePos.m_XPos
    return void();

}

void CovusControl::Stop()
{
    if(m_state_t != State_t::BUSSY)
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","state is not bussy");
        return void();
    }

    std::string l_StopStr;
    l_StopStr = char(0x3)+"\r";

    m_TCPConCorv->Send(l_StopStr);

    m_state_t = State_t::ERROR;

}



void CovusControl::Reset()
{
    if(m_state_t == State_t::BUSSY)
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","state is not Error");
        return void();
    }

    std::string l_ResetStr;
    l_ResetStr = "cal\r";
    //std::cout << "reset is running @corvuscontrol" <<  std::endl;
    m_TCPConCorv->Send(l_ResetStr);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    m_TCPConCorv->Send("st\r");

    std::string GetStateStr;
    m_TCPConCorv->Receive(GetStateStr,-1,char(0xd));
    // Timeout = -1 blocking socket has to wait till cal is finished
    auto t = std::stoi(GetStateStr);
//    GlobalLogger::Log(Logger::Type_t::WARNING, "CorvusCore",
//                          "configured"
//                          + GetStateStr);
    if( t != 0)
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"@CorvusControl","Reset Connection failed "+GetStateStr );
        return void();
    }

    m_state_t = State_t::CONFIGURED;

}

CovusControl::State_t CovusControl::GetState(Corvuspos &r_CorvPos)
{

        std::string GetStateStr;
        std::string l_GetStateStr ("st\r");

        m_TCPConCorv->Send(l_GetStateStr);

        switch(m_TCPConCorv->Receive(GetStateStr,m_CorvCtrlTimeout,char(0xd)))
        {
        case -3:
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                if(m_TCPConCorv->Receive(GetStateStr,m_CorvCtrlTimeout,char(0xd))>0)
                {
                  break;
                }
                else
                {
                   throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","Timeout -3" );
                };

            break;
        case 0:
            m_state_t = State_t::ERROR;
            throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"CorvusControl","Socket closed 0" );
            break;
        default:
            std::stoi(GetStateStr) != 0 ? m_state_t = State_t::BUSSY :  m_state_t = State_t::CONFIGURED;
            break;

        }



       std::this_thread::sleep_for(std::chrono::milliseconds(200));
       GetStateStr.clear();
       m_TCPConCorv->Send( "pos\r");
       m_TCPConCorv->Receive(GetStateStr,m_CorvCtrlTimeout,char(0xd));

       //GlobalLogger::Log(Logger::Type_t::INFO,"@Corvuscontrol ",GetStateStr);
       if(!GetStateStr.empty())
       {

           try
           {
               std::replace(GetStateStr.begin(),GetStateStr.end(),'.',',');
//               auto iFirst = std::find_if(GetStateStr.begin(), GetStateStr.end(), [](const char mychar) {return !std::isspace(mychar);});
//               auto iLast = std::find_if(iFirst, GetStateStr.end(), [](const char mychar) {return std::isspace(mychar);});
//                r_CorvPos.XPos = std::stod(std::string(iFirst, iLast));

//                iFirst = std::find_if(iLast, GetStateStr.end(), [](const char mychar) {return !std::isspace(mychar);});
//                iLast = std::find_if(iFirst, GetStateStr.end(), [](const char mychar) {return std::isspace(mychar);});

//                r_CorvPos.YPos = std::stod(std::string(iFirst, iLast));
                r_CorvPos.XPos = std::stod(GetStateStr);
//                std::getline(ss,GetStateStr,' ');
                r_CorvPos.YPos = std::stod(GetStateStr.substr(12,std::string::npos));
//                r_CorvPos.YPos = std::stod(GetStateStr);

           }catch (const std::invalid_argument& ia)
           {
                GlobalLogger::Log(Logger::Type_t::INFO,"@Corvuscontrol ",ia.what());
           }
        }


       return m_state_t;
}

void CovusControl::Finish()
{
    m_state_t = State_t::IDLE;
}
