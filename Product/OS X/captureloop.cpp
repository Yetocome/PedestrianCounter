#include<captureloop.h>
CaptureLoop::CaptureLoop(int &module)
{
    this->module = module;
}

void CaptureLoop::run()
{
    while(true){
        if(module){
            emit this->show();
        }
    }
}

