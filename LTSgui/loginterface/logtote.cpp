#include "logtote.h"
#include <sstream>
#include <QString>
#include <gui.h>

logtotE::logtotE(GUI* const pGui) noexcept
    : fpGui(pGui)
{
}

void logtotE::Write(const std::string &rMsg)noexcept
{

    auto msg = std::string(rMsg.begin(),rMsg.end()-1);
    std::stringstream ss;
    if(std::string::npos!=msg.find("WARNING"))
    {
       ss << "<font color=\"Orange\">" << msg << "</font>";
    }else if(std::string::npos!=msg.find("FATAL ERROR")&&std::string::npos!=msg.find("UNKNOWN") )
    {
        ss << "<font color=\"Red\">" << msg << "</font>";
    }else
    {
        ss << msg;
    }

    fpGui->emitLogSignal(ss.str());
//    std::lock_guard<std::mutex> logger(this->fMutex); // thread safety
//    fptE->append(QString(ss.str().c_str()));

}
