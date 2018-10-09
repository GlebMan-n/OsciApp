#include "osciapp.h"
#include "ui_osciapp.h"
#include "oscilloscope.h"
#include <QTimer>
#include <QRandomGenerator>
#include <zlogdata.h>
#include <QDateTime>
#include <QDebug>

OsciApp::OsciApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OsciApp)
{
    ui->setupUi(this);
    m_oscilloscope = new Oscilloscope();
    m_oscilloscope->addHCategory(8,QObject::tr("все"));
    m_oscilloscope->update();
    this->setCentralWidget(m_oscilloscope);
    this->grabGesture(Qt::PanGesture);
    this->grabGesture(Qt::PinchGesture);
    resize(800, 600);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
    m_timer->start(2000);
}

void OsciApp::slotTimer()
{
    QVariant qVariant = QRandomGenerator::global()->bounded(15);
    int id = QRandomGenerator::global()->bounded(3);
    //qDebug() << "идентификатор тренда: "<< id;
    ZLogData data(id, qVariant, QDateTime::currentDateTime());
    QVector<ZLogData> vdata;
    vdata.append(data);
    m_oscilloscope->slotNewData(vdata);
    m_oscilloscope->update();
}

OsciApp::~OsciApp()
{
    delete ui;
}
