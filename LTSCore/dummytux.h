#ifndef DUMMYTUX_H
#define DUMMYTUX_H
#include "config/Config_t.h"
#include "container/updateinfo.h"


class dummytux
{
public:
    enum class State_t
    {
      IDLE,CONFIGURED,BUSSY,ERROR
    };

     dummytux(std::shared_ptr<updateinfo> sptr_updateinfo);
    void Configure(Config_t* const dummyconfig);
    void Execute();
    void Finish();
    dummytux::State_t GetState();


private:
    std::shared_ptr<updateinfo> bla;
};

#endif // DUMMYTUX_H
