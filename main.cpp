#include "osciapp.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMessageBox msg;
    msg.setText(QObject::tr("+ уменьшить\n - увеличить\nF5 - автообновление\nLEFT/RIGHT прокрутка вперед и назад\nEsc - значения по умолчанию"));
    msg.exec();
    OsciApp w;
    w.show();
    return a.exec();
}
