
#include "posdraft.h"
#include "ui_posdraft.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>

posdraft::posdraft(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::posdraft)
{
    ui->setupUi(this);
   // setGeometry(0, 0, 380, 300);

    //    setBackgroundRole(QPalette::Base);
    //    setAutoFillBackground(true);


    ui->customPlot->xAxis->setTicks(true);

   // ui->customPlot->yAxis2->setTicks(false);
    //ui->customPlot->xAxis2->setVisible(false);
   // ui->customPlot->yAxis2->setVisible(false);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x [mm]");
    ui->customPlot->yAxis->setLabel("y [mm]");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0,800);
    ui->customPlot->xAxis2->setRange(0, 800);
    ui->customPlot->yAxis->setRange(0, 180);
    //ui->customPlot->yAxis->setRangeReversed(true);
    ui->customPlot->xAxis->setRangeReversed(true);
    pathcurve = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    this->ui->customPlot->addPlottable(pathcurve);
    poscurve = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    this->ui->customPlot->addPlottable(poscurve);
   // this->PlotPosition(ui->customPlot);
}

posdraft::~posdraft()
{
    delete ui;
}

void posdraft::PlotPosition(double xPos, double yPos)
{

  QVector<double> xValue(1), yValue(1);
  xValue[0]=xPos;
  yValue[0]=yPos;

  poscurve->setData(xValue, yValue);
  poscurve->setPen(QPen(Qt::red));
  poscurve->setLineStyle(QCPCurve::LineStyle::lsNone);
  poscurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));
   this->ui->customPlot->rescaleAxes(false);
  ui->customPlot->replot();

}

void posdraft::PlotPath(const std::vector<Pos_t>& path)
{

    QVector<double> x, y;

    x.resize(path.size());
    y.resize(path.size());

   for(unsigned int i =0 ; i < path.size() ; i++)
   {
       x[i] = path[i].m_XPos;
       y[i] = path[i].m_YPos;
   }
    this->pathcurve->setData(x,y);
    this->pathcurve->setPen(QPen(Qt::blue));
    this->pathcurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 5));

    this->ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    //this->ui->customPlot->axisRect()->setupFullAxesBox();
    this->ui->customPlot->rescaleAxes();
    this->ui->customPlot->replot();
}
