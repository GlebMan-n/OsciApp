#include "osciapp.h"
#include "ui_osciapp.h"
#include "oscilloscope.h"
#include <QTimer>
#include <QRandomGenerator>
#include <zlogdata.h>
#include <QDateTime>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QLayout>

OsciApp::OsciApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OsciApp)
{
    ui->setupUi(this);
    m_size = 5;
    createTestData();
    this->grabGesture(Qt::PanGesture);
    this->grabGesture(Qt::PinchGesture);
    resize(800, 600);
    m_timeout = 500;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
    m_timer->start(m_timeout);
    /*m_file = menuBar()->addMenu(QObject::tr("&File"));
    m_view = menuBar()->addMenu(QObject::tr("&View"));
    m_test = menuBar()->addMenu(QObject::tr("&Test"));*/
    m_help = menuBar()->addMenu(QObject::tr("&Help"));
    m_helpAction = new QAction(tr("&Help"), this);
    connect(m_helpAction, &QAction::triggered, this, &OsciApp::slotHelp);

    /*m_zoomInAction = new QAction(tr("&Zoom In"), this);
    connect(m_zoomInAction, &QAction::triggered, this, &OsciApp::slotZoomIn);
    m_zoomOutAction = new QAction(tr("&Zoom Out"), this);
    connect(m_zoomOutAction, &QAction::triggered, this, &OsciApp::slotZoomOut);
    m_stopTimerAction = new QAction(tr("&Stop timer"), this);
    connect(m_stopTimerAction, &QAction::triggered, this, &OsciApp::slotStopTimer);
    m_startTimerAction = new QAction(tr("&Start timer"), this);
    connect(m_startTimerAction, &QAction::triggered, this, &OsciApp::slotStartTimer);
    m_setTimerIntervalAction = new QAction(tr("&Interval"), this);
    connect(m_setTimerIntervalAction, &QAction::triggered, this, &OsciApp::slotSetTimerInterval);

    m_autoUpdateAction = new QAction(tr("&Auto update"), this);
    connect(m_autoUpdateAction, &QAction::triggered, this, &OsciApp::slotAutoUpdate);
    m_addVCatAction = new QAction(tr("&Vertical cat"), this);
    connect(m_addVCatAction, &QAction::triggered, this, &OsciApp::slotAddVCat);
    m_addHCatAction = new QAction(tr("&Horizontal cat"), this);
    connect(m_addHCatAction, &QAction::triggered, this, &OsciApp::slotAddHCat);
    m_refreshAction = new QAction(tr("&Refresh"), this);
    connect(m_refreshAction, &QAction::triggered, this, &OsciApp::slotRefresh);*/

    /*m_file->addAction(m_refreshAction);
    m_test->addAction(m_setTimerIntervalAction);
    m_test->addAction(m_startTimerAction);
    m_test->addAction(m_stopTimerAction);
    m_test->addAction(m_addVCatAction);
    m_test->addAction(m_addHCatAction);

    m_view->addAction(m_zoomInAction);
    m_view->addAction(m_zoomOutAction);*/
    m_help->addAction(m_helpAction);
}

void OsciApp::createTestData()
{
    for(auto i = 0; i < m_size; i++)
    {
        Oscilloscope* osci = new Oscilloscope(i);
        osci->addVCategory(600,tr(""));
        m_oscilloscopes.append(osci);
        this->centralWidget()->layout()->addWidget(osci);
        osci->show();
    }
}

Oscilloscope* OsciApp::getOsciById(int id)
{
    for(auto i = 0; i < m_oscilloscopes.size(); i++)
        if(m_oscilloscopes.at(i)->getId() == id)
            return  m_oscilloscopes.at(i);
    return nullptr;
}

void OsciApp::slotTimer()
{
    QVariant qVariant = QRandomGenerator::global()->bounded(15);
    int id = QRandomGenerator::global()->bounded(m_size);
    ZLogData data(id, qVariant, QDateTime::currentDateTime());
    QVector<ZLogData> vdata;
    vdata.append(data);
    Oscilloscope* osci = getOsciById(id);
    if(!osci)
        return;
    osci->slotNewData(vdata);
    osci->update();
}

OsciApp::~OsciApp()
{
    delete ui;
}


void OsciApp::slotZoomIn()
{
    /*if(m_oscilloscope)
        m_oscilloscope->zoomIn();*/
}
void OsciApp::slotZoomOut()
{
    /*if(m_oscilloscope)
        m_oscilloscope->zoomOut();*/
}
void OsciApp::slotStopTimer()
{
    if(m_timer)
        m_timer->stop();
}
void OsciApp::slotStartTimer()
{
    if(m_timer)
        m_timer->start(m_timeout);
}
void OsciApp::slotSetTimerInterval()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Задайте интервал таймера"),
                                tr("сек:"), m_timeout/1000, 0, 60, 1, &ok);
    if (ok)
    {
       m_timeout = i*1000;
       m_timer->start(m_timeout);
    }
}
void OsciApp::slotHelp()
{
    QMessageBox msg;
    msg.setText(QObject::tr("+ уменьшить\n - увеличить\nF5 - автообновление\nLEFT/RIGHT прокрутка вперед и назад\nEsc - значения по умолчанию\nЗажать Ctrl для зума"));
    msg.exec();
}
void OsciApp::slotAutoUpdate()
{
    /*if(m_oscilloscope)
        m_oscilloscope->autoupdate();*/
}
void OsciApp::slotAddVCat()
{
    /*if(m_oscilloscope)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Текст категории"),tr("Текст"), QLineEdit::Normal,tr("нет"),&ok);
        if(!ok)
            return;
        int i = QInputDialog::getInt(this, tr("Задайте положение категории"),
                                    tr("положение:"), m_oscilloscope->getTimeMax() / 2.0, m_oscilloscope->getTimeMin(), m_oscilloscope->getTimeMax(), 1, &ok);
        if(!ok)
            return;
        m_oscilloscope->addVCategory(i,text);
        m_oscilloscope->update();
    }*/
}
void OsciApp::slotAddHCat()
{
   /* bool ok;
    QString text = QInputDialog::getText(this, tr("Текст категории"),tr("Текст"), QLineEdit::Normal,tr("нет"),&ok);
    if(!ok)
        return;
    int i = QInputDialog::getInt(this, tr("Задайте положение категории"),
                                tr("положение:"), m_oscilloscope->getValMax() / 2.0, m_oscilloscope->getValMin(), m_oscilloscope->getValMax(), 1, &ok);
    if(!ok)
        return;
    m_oscilloscope->addHCategory(i,text);
    m_oscilloscope->update();*/
}

void OsciApp::slotRefresh()
{
    /*if(m_oscilloscope)
        m_oscilloscope->refresh();*/
}
