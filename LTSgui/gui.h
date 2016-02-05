#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "subgui/corvusverification.h"
#include "subgui/posdraft.h"
#include "../LTSCore/config/configreader.h"
#include "../LTSCore/config/Config_t.h"
#include "loginterface/logtote.h"
#include "../LTSCore/log/Logger.h"
#include "../LTSCore/ltscore.h"
#include "../LTSCore/container/updateinfo.h"
#include <QTimer>

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();
    QTextEdit *rTE_Log() const noexcept;
    void SelectChangeOver(const CorvusVerification::Transition_t trans);
    void Configure();
    void Start();
    void Stop();
    void Continue();
    void Reset();
    void updateinfotTimer();

private:
    Ui::GUI *ui;
   // Logger log;
    //Config_t m_ConfigFile;
    Logger m_Logger;
    //std::unique_ptr<Logger> m_uptr_Logger;
    std::unique_ptr<LTSCore> m_uptr_Corvus;
    std::shared_ptr<updateinfo> m_sptr_update;

    QTimer *ptr_timer;

   // unsigned int counter=0;
signals:
    void SignalLog(const QString& MSG);

public:
    inline void setTimer()
    {
        if(!(ptr_timer->isActive()))
        this->ptr_timer->start(500);
    }
    inline void stopTimer()
    {
        if(ptr_timer->isActive())
        this->ptr_timer->stop();
    }

    inline void emitLogSignal(const std::string& MSG) noexcept
    {
        emit SignalLog(QString(MSG.c_str()));
    }
};

#endif // GUI_H
