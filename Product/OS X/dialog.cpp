#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include<iostream>
using namespace std;
Dialog::Dialog(QImage image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint); //隐藏菜单栏
    this->move((desktop.availableGeometry().width()-this->width()),desktop.availableGeometry().height());//初始化位置到右下角
    //
    ui->label->setPixmap(QPixmap::fromImage(image));
    //
    showAnimation(); //开始显示右下角弹出框
}

Dialog::~Dialog()
{
    delete ui;
}
//弹出动画
void Dialog::showAnimation(){
    //显示弹出框动画
    animation=new QPropertyAnimation(this,"pos");
    animation->setDuration(2000);
    animation->setStartValue(QPoint(this->x(),this->y()));
    animation->setEndValue(QPoint((desktop.availableGeometry().width()-this->width()),(desktop.availableGeometry().height()-this->height())));
    animation->start();

    //设置弹出框显示2秒、在弹回去
    remainTimer=new QTimer();
    connect(remainTimer,SIGNAL(timeout()),this,SLOT(closeAnimation()));
    remainTimer->start(4000);//弹出动画2S,停留2S回去
}
//关闭动画
void Dialog::closeAnimation(){
    //清除Timer指针和信号槽
    remainTimer->stop();
    disconnect(remainTimer,SIGNAL(timeout()),this,SLOT(closeAnimation()));
    delete remainTimer;
    remainTimer=NULL;
    //弹出框回去动画
    animation->setStartValue(QPoint(this->x(),this->y()));
    animation->setEndValue(QPoint((desktop.availableGeometry().width()-this->width()),desktop.availableGeometry().height()));
    animation->start();
    //弹回动画完成后清理动画指针
    connect(animation,SIGNAL(finished()),this,SLOT(clearAll()));
}
//清理动画指针
void Dialog::clearAll(){
    disconnect(animation,SIGNAL(finished()),this,SLOT(clearAll()));
    delete animation;
    animation=NULL;
    this->close();
}
