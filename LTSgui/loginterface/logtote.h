#ifndef LOGTOTE_H
#define LOGTOTE_H

#include <mutex>
#include <QTextEdit>

class GUI;

class logtotE
{
public:
    logtotE(GUI* const pGui) noexcept;
    void Write(const std::string& rMsg) noexcept;

signals:
    void SignalLog(const std::string MSG);

private:
    GUI* const fpGui;
//    QTextEdit *const fptE;
//    std::mutex fMutex;

};

#endif // LOGTOTE_H
