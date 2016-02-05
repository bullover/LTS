#ifndef CORVUSVERIFICATION_H
#define CORVUSVERIFICATION_H

#include <QWidget>

#include "../LTSCore/container/updateinfo.h"

namespace Ui {
class CorvusVerification;
}

class CorvusVerification : public QWidget
{
    Q_OBJECT

public:
    enum class Transition_t
    {
        CONFIGURE,
        START,
        STOP,
        CONTINUE,
        RESET,
        EXIT,
        ERROR

    };

public:
    explicit CorvusVerification(QWidget *parent = 0);
    ~CorvusVerification();

    void FileDialog();
    std::string XMLFilePath();
    void SelectpBEnable(updateinfo::State_t_LTSCtrl State);
    void pBIdeled();
    void pBConfigured();
    void pBStarted();
    void pBStoped();
    void pBErrored();
    void pBBussied();

signals:
    void SignalTransition(const Transition_t Statetransition);

private: // functions emitting signals


    inline void EmitConfigure() noexcept
    {

        emit this->SignalTransition(Transition_t::CONFIGURE);
        this->pBBussied();
    }

    inline void EmitSart() noexcept
    {

        emit this->SignalTransition(Transition_t::START);
        this->pBBussied();
    }

    inline void EmitStop() noexcept
    {

        emit this->SignalTransition(Transition_t::STOP);
        this->pBBussied();
    }

    inline void EmitContinue() noexcept
    {

        emit this->SignalTransition(Transition_t::CONTINUE);
        this->pBBussied();
    }

    inline void EmitExit() noexcept
    {

        emit this->SignalTransition(Transition_t::EXIT);
        this->pBBussied();
    }

    inline void EmitReset() noexcept
    {

        emit this->SignalTransition(Transition_t::RESET);
        this->pBBussied();
    }


private:
    Ui::CorvusVerification *ui;
};

#endif // CORVUSVERIFICATION_H
