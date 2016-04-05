#include "qwidgetdynamic.h"
#include "ui_qwidgetdynamic.h"

QWidgetDynamic::QWidgetDynamic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWidgetDynamic)
{
    ui->setupUi(this);
    for(int i = 0;i<MAX;i++){
        point[i].ry() = 0;
    }
}

void QWidgetDynamic::paintEvent(QPaintEvent *){
    QPainter *painter;
    painter = new QPainter(this);
    QPen pen = painter->pen();
    pen.setColor(Qt::red);
    pen.setWidth(3);
    painter->setPen(pen);
    for(int i = 0;i<MAX;i++){
        point[i].rx() = i*this->width()/(MAX-1);
    }

//    for(int i = 0;i<MAX;i++){
//        painter->drawEllipse(point[i].rx(),this->height() - point[i].ry(),1,1);
//    }
    for(int i = 0;i<(MAX-1);i++){
        painter->drawLine(QPoint(point[i].rx(),this->height() - point[i].ry()),
                          QPoint(point[i+1].rx(),this->height() - point[i+1].ry()));
    }
}
void QWidgetDynamic::setPoint(int num){
    if(count_point == MAX){
        for(int i = 0; i<MAX ;i++){
            point[i].ry() = 0;
        }
        count_point = 0;
    }
    point[count_point].ry() = num*this->height()/10;
    this->update();
    count_point++;
}

void QWidgetDynamic::clear(){
    for(int i = 0;i<MAX;i++){
        point[i].ry() = 0;
    }
    this->update();
    count_point = 0;
}

QWidgetDynamic::~QWidgetDynamic()
{
    delete ui;
}
