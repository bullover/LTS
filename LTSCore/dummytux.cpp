#include "dummytux.h"
#include <thread>
#include <chrono>


dummytux::dummytux(std::shared_ptr<updateinfo> sptr_updateinfo):bla(sptr_updateinfo)
{
}

void dummytux::Configure(Config_t * const dummyconfig)
{

}

void dummytux::Execute()
{
     std::this_thread::sleep_for(std::chrono::seconds(2));
}

void dummytux::Finish()
{

}

dummytux::State_t dummytux::GetState(){return State_t::CONFIGURED;}
