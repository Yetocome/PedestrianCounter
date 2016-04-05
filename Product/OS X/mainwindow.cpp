#include "mainwindow.h"
#include<QPainter>
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setMouseTracking(true);
    module = 0;
    max_number = 0;
    count_number = 0;
    current_number = 0;
    show_loop = new CaptureLoop(module);
    connect(show_loop,SIGNAL(show()),this,SLOT(show_data()));
    show_loop->start();

}
void MainWindow::show_data(){
    cout<<"+++++++++++++++"<<endl;
    ui->lcdNumber_count->display(count_number);
    ui->lcdNumber_current->display(current_number);
    ui->lcdNumber_max->display(max_number);
}

//打开摄像头
void MainWindow::on_actionOpen_triggered(){
    module = 1;
    VideoCapture cap(0);
    if (!cap.isOpened()){
        return;
    }
    //Mat img, show_img;
    int lost_frame = 0;
    while (true) {
        cap >> img;
        if (img.data) {
            break;
        }
    }
    Size capture_size = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CV_CAP_PROP_FPS);
    VideoWriter writer("test.avi", -1, fps,capture_size);
    PDCounter pc(img);
    pc.showTrajectorySwitch();
    pc.showAreasSwitch();
    namedWindow("Video", CV_WINDOW_AUTOSIZE);
    int start_frame = 0;
    int i = 0;
    while (module == 1) {
        cap >> img;
        writer << img;
        if (i++  < start_frame) {
            continue;
        }
        if (cvWaitKey(10) == 27)
            break;
        if (!img.data && lost_frame < 3) {
            lost_frame++;
            continue;
        } else if (lost_frame >= 3) {
            break;
        } else {
            lost_frame = 0;
        }
        show_img = pc.detect(img);
        current_number = pc.getCurrNum();
        ui->widgetDynamic->setPoint(current_number);

//        cout << pc.getLog() << endl;
//        cout << "Current Data:" << endl;
//        cout << "CurrNum:" << pc.getCurrNum() << endl; // Return the number of pedestrians on the screen
//        cout << "PosPastNum:" << pc.getPosPastNum(0) << endl;
//        cout << "NegPastNum:" << pc.getNegPastNum(0) << endl;
//        cout << "GhostNum:" << pc.getGhostNum(0) << endl;
//        cout << "LostNum:" << pc.getLostNum() << endl;
        imshow("Video", show_img);
        ui->lcdNumber_count->display(count_number);
        ui->lcdNumber_current->display(current_number);
        ui->lcdNumber_max->display(max_number);
        }
    writer.release();
    destroyWindow("Video");
}

//网络模式
void MainWindow::on_actionNet_triggered()
{
    module = 2;
    QString filename;
    filename = QInputDialog::getText(this,tr("网络地址"),tr("地址："));
    if(filename.isEmpty()){
        cout<<"filename.isEmpty()"<<endl;
        return;
    }
    string str = filename.toStdString();
    //const string str = "http://192.168.1.1:8080/?action=stream?dummy=param.mjpg";
    VideoCapture cap(str);
    if (!cap.isOpened()){
        cout<<"cap empty"<<endl;
        return;
    }
    int lost_frame = 0;
    while (true) {
        cap >> img;
        if (img.data) {
            break;
        }
    }
    PDCounter pc(img);
    pc.showTrajectorySwitch();
    pc.showAreasSwitch();
    namedWindow("Video", CV_WINDOW_AUTOSIZE);
    int start_frame = 0;
    int i = 0;
    while (module == 2) {
        cap >> img;
        if (i++  < start_frame) {
            continue;
        }
        if (cvWaitKey(10) == 27)
            break;
        if (!img.data && lost_frame < 3) {
            lost_frame++;
            continue;
        } else if (lost_frame >= 3) {
            break;
        } else {
            lost_frame = 0;
        }
        show_img = pc.detect(img);
        cout << pc.getLog() << endl;
        cout << "Current Data:" << endl;
        cout << "CurrNum:" << pc.getCurrNum() << endl; // Return the number of pedestrians on the screen
        cout << "PosPastNum:" << pc.getPosPastNum(0) << endl;
        cout << "NegPastNum:" << pc.getNegPastNum(0) << endl;
        cout << "GhostNum:" << pc.getGhostNum(0) << endl;
        cout << "LostNum:" << pc.getLostNum() << endl;
        imshow("Video", show_img);
    }
    destroyWindow("Video");
}


//打开本地录像
void MainWindow::on_actionFile_triggered(){
    module = 3;//标明模式
    QString filename;
    filename = QFileDialog::getOpenFileName(this,tr("选择视频"),"",tr("媒体文件 (*.*)"));
    if(filename.isEmpty()){
        cout<<"filename.isEmpty()"<<endl;
        return;
    }
    string str = filename.toStdString();
    VideoCapture cap(str);
    if(!cap.isOpened()){
        QMessageBox::information(this,tr("错误信息"),tr("打开本地视频失败"));
        return;
    }
    int lost_frame = 0;
    while (true) {
        cap >> img;
        if (img.data) {
            break;
        }
    }
    PDCounter pc(img);
    pc.showTrajectorySwitch();
    pc.showAreasSwitch();
    namedWindow("Video", CV_WINDOW_AUTOSIZE);
    int start_frame = 0;
    int i = 0;
    while (module == 3) { // Press any key to exit
        /* code */
        cap >> img;
        if (i++  < start_frame) {
            continue;
        }
        if (cvWaitKey(10) == 27)
            break;
        if (!img.data && lost_frame < 3) {
            lost_frame++;
            continue;
        } else if (lost_frame >= 3) {
            break;
        } else {
            lost_frame = 0;
        }
        show_img = pc.detect(img);
        cout << pc.getLog() << endl;
        cout << "Current Data:" << endl;
        cout << "CurrNum:" << pc.getCurrNum() << endl; // Return the number of pedestrians on the screen
        cout << "PosPastNum:" << pc.getPosPastNum(0) << endl;
        cout << "NegPastNum:" << pc.getNegPastNum(0) << endl;
        cout << "GhostNum:" << pc.getGhostNum(0) << endl;
        cout << "LostNum:" << pc.getLostNum() << endl;
        imshow("Video", show_img);
        }
    destroyWindow("Video");
}

//截屏 保存
void MainWindow::on_actionslice_triggered(){
    if(module == 0){
        ui->statusBar->showMessage(tr("无视频文件"), 5000);
        return ;
    }
    if(show_img.empty()){
        return;
    }
    QImage image;
    Mat mat_tmp;
    cvtColor(show_img,mat_tmp,CV_BGR2RGB);
    image = QImage((const uchar*)(mat_tmp.data),mat_tmp.cols,mat_tmp.rows,mat_tmp.cols*mat_tmp.channels(),QImage::Format_RGB888);
    ui->statusBar->showMessage(tr("正在捕获图片"), 1000);
    QString fileName=QFileDialog::getSaveFileName(this, tr("保存到文件"), QDir::homePath(), tr("jpeg格式文件(*.jpg)"));
    if(fileName.isEmpty()) {
        ui->statusBar->showMessage(tr("保存操作已取消"), 5000);
        return;
    }
    //截屏照片展示
    Dialog dialog(image);
    ui->statusBar->showMessage(tr("捕获成功"), 5000);
    dialog.show();
    image.save(fileName);
    ui->statusBar->showMessage(tr("保存成功"), 5000);

}


//帮助
void MainWindow::on_actionIntroduction_triggered()
{
    QMessageBox::information(this,tr("功能简介"),tr(" "));
}

void MainWindow::on_actionVersion_triggered()
{
    QMessageBox::information(this,tr("版本信息"),tr(" "));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,tr("关于我们"),tr(" "));
}

//侧边栏隐藏效果
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{

    if(e->x()<64){
        ui->toolBar->show();
    }
    else{
        ui->toolBar->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    module = 0;
    ui->statusBar->showMessage(tr("关闭程序中..."),1000);
}

MainWindow::~MainWindow(){

    delete ui;
}
