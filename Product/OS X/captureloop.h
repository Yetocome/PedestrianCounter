#ifndef CAPTURELOOP
#define CAPTURELOOP
#include<QThread>

class CaptureLoop :public QThread
{
    Q_OBJECT
public:
    CaptureLoop(int &module);
protected:
    void run();
private:
    int module;
signals:
    void show();
};

#endif // CAPTURELOOP

