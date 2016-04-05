#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://image/camera.png"));
    MainWindow w;
    w.setWindowTitle("行 人 检 测");
    w.show();
    return a.exec();
}
