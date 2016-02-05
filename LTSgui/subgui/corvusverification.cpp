#include "corvusverification.h"
#include "ui_corvusverification.h"

#include <QFileDialog>
#include "../LTSCore/log/Logger.h"

CorvusVerification::CorvusVerification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CorvusVerification)
{
    ui->setupUi(this);
    QObject::connect(ui->pB_LoadConfigure,&QPushButton::clicked,
                     this,&CorvusVerification::FileDialog);
    QObject::connect(ui->pB_Configure,&QPushButton::clicked,
                     this,&CorvusVerification::EmitConfigure);
    QObject::connect(ui->pB_Start,&QPushButton::clicked,
                     this,&CorvusVerification::EmitSart);
    QObject::connect(ui->pB_Stop,&QPushButton::clicked,
                     this,&CorvusVerification::EmitStop);
    QObject::connect(ui->pB_Continue,&QPushButton::clicked,
                     this,&CorvusVerification::EmitContinue);
    QObject::connect(ui->pB_Reset,&QPushButton::clicked,
                     this,&CorvusVerification::EmitReset);


}

CorvusVerification::~CorvusVerification()
{
    delete ui;
}

void CorvusVerification::FileDialog()
{

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    this->ui->LE_FilePath->setText(dialog.getOpenFileName());

}

std::string CorvusVerification::XMLFilePath()
{
    return this->ui->LE_FilePath->text().toStdString();
}

void CorvusVerification::SelectpBEnable(updateinfo::State_t_LTSCtrl State)
{
    switch (State) {
    case updateinfo::State_t_LTSCtrl::IDLE:
        this->pBIdeled();
        break;
    case updateinfo::State_t_LTSCtrl::CONFIGURED:
        this->pBConfigured();
        break;
    case updateinfo::State_t_LTSCtrl::RUNNING:
        this->pBStarted();
        break;
    case updateinfo::State_t_LTSCtrl::STOPPED:
        this->pBStoped();
        break;
    case updateinfo::State_t_LTSCtrl::ERROR:
        this->pBErrored();
        break;
    case updateinfo::State_t_LTSCtrl::BUSSY:
        this->pBBussied();
        break;
    default:
        break;
    }
}


void CorvusVerification::pBIdeled()
{
    ui->pB_LoadConfigure->setEnabled(true);
    ui->pB_Configure->setEnabled(true);
    ui->pB_Start->setEnabled(false);
    ui->pB_Stop->setEnabled(false);
    ui->pB_Continue->setEnabled(false);
    ui->pB_Reset->setEnabled(false);

}

void CorvusVerification::pBConfigured()
{
    ui->pB_LoadConfigure->setEnabled(true);
    ui->pB_Configure->setEnabled(true);
    ui->pB_Start->setEnabled(true);
    ui->pB_Stop->setEnabled(false);
    ui->pB_Continue->setEnabled(true);
    ui->pB_Reset->setEnabled(false);
}
void CorvusVerification::pBStarted()
{
    ui->pB_LoadConfigure->setEnabled(true);
    ui->pB_Configure->setEnabled(false);
    ui->pB_Start->setEnabled(false);
    ui->pB_Stop->setEnabled(true);
    ui->pB_Continue->setEnabled(false);
    ui->pB_Reset->setEnabled(false);
}

void CorvusVerification::pBStoped()
{
    ui->pB_LoadConfigure->setEnabled(true);
    ui->pB_Configure->setEnabled(true);
    ui->pB_Start->setEnabled(false);
    ui->pB_Stop->setEnabled(false);
    ui->pB_Continue->setEnabled(true);
    ui->pB_Reset->setEnabled(true);
}

void CorvusVerification::pBErrored()
{
    ui->pB_LoadConfigure->setEnabled(true);
    ui->pB_Configure->setEnabled(true);
    ui->pB_Start->setEnabled(false);
    ui->pB_Stop->setEnabled(false);
    ui->pB_Continue->setEnabled(false);
    ui->pB_Reset->setEnabled(false);
}
void CorvusVerification::pBBussied()
{
    ui->pB_LoadConfigure->setEnabled(false);
    ui->pB_Configure->setEnabled(false);
    ui->pB_Start->setEnabled(false);
    ui->pB_Stop->setEnabled(false);
    ui->pB_Continue->setEnabled(false);
    ui->pB_Reset->setEnabled(false);
}
