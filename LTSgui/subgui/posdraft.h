#ifndef POSDRAFT_H
#define POSDRAFT_H

#include <QWidget>

#include "qcustomplot/qcustomplot.h"
#include "../LTSCore/config/Config_t.h"

namespace Ui {
class posdraft;
}

class posdraft : public QWidget
{
    Q_OBJECT

public:
    explicit posdraft(QWidget *parent = 0);
    ~posdraft();
    void PlotPosition(double xPos, double yPos);
    void PlotPath(const std::vector<Pos_t> &path);
private:
    Ui::posdraft *ui;
    QCPCurve *poscurve ;
    QCPCurve *pathcurve ;
};

#endif // POSDRAFT_H
