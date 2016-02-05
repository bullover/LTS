#include "gui.h"
#include "ui_gui.h"
#include <iostream>
#include <QFileDialog>
#include <QString>

GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI),m_Logger(),m_uptr_Corvus(nullptr),m_sptr_update(nullptr),ptr_timer(nullptr)
{
    ui->setupUi(this);
    m_Logger.Add<logtotE>(this);
    this->m_sptr_update = std::make_shared<updateinfo>();
    this->m_uptr_Corvus = std::make_unique<LTSCore>(this->m_sptr_update);
    this->ptr_timer=new QTimer(this);
    QObject::connect(ui->pB_ClearLog,&QPushButton::clicked,ui->TE_Log,&QTextEdit::clear);
    QObject::connect(ui->CorvusCntrl_WD,&CorvusVerification::SignalTransition,this,&GUI::SelectChangeOver);
    QObject::connect(ptr_timer, &QTimer::timeout, this, &GUI::updateinfotTimer,Qt::UniqueConnection);
    QObject::connect(this,&GUI::SignalLog,ui->TE_Log,&QTextEdit::append);

}

GUI::~GUI()
{
    this->m_uptr_Corvus->SetStateReq(LTSCore::Transition_t::EXIT);
    this->stopTimer();
    delete ui;
}

QTextEdit *GUI::rTE_Log() const noexcept
{
    return ui->TE_Log;
}

void GUI::SelectChangeOver(const CorvusVerification::Transition_t trans)
{
    switch (trans)
    {
    case CorvusVerification::Transition_t::CONFIGURE:
        this->Configure();
        break;
    case CorvusVerification::Transition_t::START:
        this->Start();
        break;
    case CorvusVerification::Transition_t::STOP:
        this->Stop();
        break;
    case CorvusVerification::Transition_t::CONTINUE:
        this->Continue();
        break;
    case CorvusVerification::Transition_t::RESET:
        this->Reset();
        break;
    default:
        break;
    }
}
//TODO LTS STATE im update container und Error state handling .....

void GUI::Configure()
{

    Config_t ConfigFile;
    try{
        Configreader XMLFile(ui->CorvusCntrl_WD->XMLFilePath());
        XMLFile.loadConfig(ConfigFile);

        if(ConfigFile.m_isValid)
        {
            ui->PathDraft_WD->PlotPath(ConfigFile.m_Corvus.m_allPosabsolut);



            this->m_uptr_Corvus->SetConfigCore(ConfigFile);
            //this->m_uptr_Corvus->Set_smart_ptrLTSCore(this->m_sptr_update);
            this->setTimer();

            this->m_uptr_Corvus->SetStateReq(LTSCore::Transition_t::CONFIGURE);
            //std::cout << ui->CorvusCntrl_WD->XMLFilePath() << " - Files" << std::endl;
        }
    }catch(const  LTS::LTSError& rError)
    {
        this->ui->CorvusCntrl_WD->SelectpBEnable(updateinfo::State_t_LTSCtrl::IDLE);
        GlobalLogger::Log(Logger::Type_t::FATAL, "LTSctrl",rError.What());
    }

}

void GUI::Start()
{
    this->m_uptr_Corvus->SetStateReq(LTSCore::Transition_t::START);

}


void GUI::Stop()
{
    this->m_uptr_Corvus->SetStateReq(LTSCore::Transition_t::STOP);

}



void GUI::Continue()
{
    this->m_uptr_Corvus->SetStateReq(LTSCore::Transition_t::CONTINUE);
}



void GUI::Reset()
{

   this->m_uptr_Corvus->SetStateReq(LTSCore::Transition_t::RESET);

}



void GUI::updateinfotTimer()
{
    Corvuspos XYPos;
    //std::mutex t_mutex;


    XYPos = this->m_sptr_update->GetPos();
    this->ui->PathDraft_WD->PlotPosition(XYPos.XPos,XYPos.YPos);
   //if(this->m_sptr_update->GetSubState()==updateinfo::Substate_t::READY)
   // {
        this->ui->CorvusCntrl_WD->SelectpBEnable(this->m_sptr_update->GetLTSState());

    //}


//    std::stringstream ss;
//    ss << "x: " << XYPos.XPos << " y: " <<XYPos.YPos;

    //std::lock_guard<std::mutex> lock(t_mutex);
   // this->ui->TE_Pos->setText(QString::fromStdString(ss.str()));
    this->ui->IN_x->display(XYPos.XPos);
    this->ui->IN_y->display(XYPos.YPos);

}
