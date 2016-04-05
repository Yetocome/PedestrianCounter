#ifndef QWIDGETDYNAMIC_H
#define QWIDGETDYNAMIC_H

#include <QWidget>
#include<QPainter>
#include<QPen>
#define MAX 100
namespace Ui {
class QWidgetDynamic;
}

class QWidgetDynamic : public QWidget
{
    Q_OBJECT

public:
    explicit QWidgetDynamic(QWidget *parent = 0);
    void setPoint(int num);
    void clear();
    ~QWidgetDynamic();
protected:
    void paintEvent(QPaintEvent *);
private:
    int count_point = 0;
    QPoint point[MAX];
    Ui::QWidgetDynamic *ui;
    bool flag = true;
};

#endif // QWIDGETDYNAMIC_H
