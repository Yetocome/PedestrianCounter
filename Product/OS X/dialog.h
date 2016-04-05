#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QPoint>
#include <QTimer>

#include <QGuiApplication>
#include <QApplication>
#include <QAction>
#include <QButtonGroup>
#include <QDialog>
#include <QHeaderView>
#include <QLabel>
//#include <mainwindow.h>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QImage image, QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QDesktopWidget desktop;
    QPropertyAnimation* animation;
    QTimer *remainTimer;
    void showAnimation();
private slots:
    void closeAnimation();
    void clearAll();
};

#endif // DIALOG_H
