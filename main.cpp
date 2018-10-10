#include "osciapp.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OsciApp w;
    w.show();
    return a.exec();
}
