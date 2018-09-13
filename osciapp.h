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

private slots:
    void slotTimer();

private:
    Ui::OsciApp *ui;
    QTimer* m_timer;
    Oscilloscope* m_oscilloscope;
};

#endif // OSCIAPP_H
