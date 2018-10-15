#ifndef OSCIAPP_H
#define OSCIAPP_H

#include <QMainWindow>

namespace Ui {
class OsciApp;
}

class QTimer;
class Oscilloscope;

class OsciApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit OsciApp(QWidget *parent = nullptr);
    ~OsciApp();
    void createTestData();
    Oscilloscope* getOsciById(int id);
private:
    QMenu*      m_file;
    QMenu*      m_view;
    QMenu*      m_test;
    QMenu*      m_help;

    QAction*    m_zoomInAction;
    QAction*    m_zoomOutAction;
    QAction*    m_stopTimerAction;
    QAction*    m_startTimerAction;
    QAction*    m_setTimerIntervalAction;
    QAction*    m_helpAction;
    QAction*    m_autoUpdateAction;
    QAction*    m_addVCatAction;
    QAction*    m_addHCatAction;
    QAction*    m_refreshAction;

private slots:
    void slotTimer();
    void slotZoomIn();
    void slotZoomOut();
    void slotStopTimer();
    void slotStartTimer();
    void slotSetTimerInterval();
    void slotHelp();
    void slotAutoUpdate();
    void slotAddVCat();
    void slotAddHCat();
    void slotRefresh();



private:
    Ui::OsciApp*            ui;
    QTimer*                 m_timer;
    QList<Oscilloscope*>    m_oscilloscopes;
    int                     m_size;
    int                     m_timeout;
};

#endif // OSCIAPP_H
