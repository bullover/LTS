#include "gui.h"
#include <QApplication>
#include "../LTSCore/log/Logger.h"
#include "loginterface/logtote.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI w;
    w.show();
    auto& rLogger = GlobalLogger::GetLogger();
        rLogger.Add<logtotE>(&w);

        GlobalLogger::Log(Logger::Type_t::INFO, "MAIN", "is runnging");
    return a.exec();
}
