#ifndef WIDGET_H
#define WIDGET_H

#include<QMap>
#include<QPainter>
#include<QImage>
#include<math.h>
#include<QWidget>
#include<QDebug>
#include<QLabel>
#include<QImage>
#include<QPixmap>
#include<QPushButton>
#include<QMenu>
#include<QFileDialog>
#include<QMenuBar>
#include<QMessageBox>
#include<QToolBar>
#include<QAction>
#include<QInputDialog>
#include<QTextEdit>
#include<QPoint>
#include<QScreen>
#include<QGridLayout>
#include<map>
#include"opencv.hpp"
#include"MyComplex.h"
#include"imagesettings.h"
#include"captureWidget.h"
#include"convertMat2QPixmap.h"
#include"node.h"
#define PI 3.1415926535
using namespace cv;
class Widget : public QWidget
{
    Q_OBJECT
private:
    bool isCapture = false;
    bool isMove = false;
    QPixmap afterPixmap;
    QPixmap beforePixmap;
    QPixmap otherPixmap;
    ImageSettings* setter;
    Mat tempMat;
    Mat img;
    Mat showMat;
    QLabel* originalLabel;
    QLabel* processedLabel;

    QPushButton* LinearMapBtn;
    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* transformMenu;
    QMenu* enhancementMenu;
    QMenu* encodeAndDivisionMenu;

    MyComplex<double>* tdComplex;
    MyComplex<double>* fdComplex;

    QPoint m_beginMouse;
    QPoint m_endMouse;
    QPoint m_globalBeginMouse;
    QPoint m_globalEndMouse;
    QPainter m_painter;
    QMap<int,string> *m_map;
private:
    void FFT(MyComplex<double>* TD, MyComplex<double>* FD, int r);
    void Fourier(int nr,int nc,MyComplex<double>*tdComplex,MyComplex<double>*fdComplex);
    void InverseFourier(int nr,int nc,MyComplex<double>*tdComplex,MyComplex<double>*fdComplex);
    void DisCosine(int nr,int nc,MyComplex<double>*tdComplex,MyComplex<double>*fdComplex);
    void InverseDisCosine(int nr,int nc,MyComplex<double>*tdComplex,MyComplex<double>*fdComplex);
    void FDCT(MyComplex<double>* TD, MyComplex<double>* FD, int r);
    void Walsh(int nr,int nc,MyComplex<double>*tdComplex,MyComplex<double>*fdComplex);
    void FWT(MyComplex<double>* TD, MyComplex<double>* FD, int r);
    int getPowerPoint(int x);
    int getClosestPower(int x);
    int linearTrans(int a,int b,int c,int d,int f);
    int logTrans(float a, float b,float c,int pixel);
    QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
    double generateGaussianNoise(double mu, double sigma);
    void sendCaptureSignal();
    Mat AddNoise(Mat img,double mu, double sigma,int k);
    Mat AddNoise(Mat img,double SNR );
    bool cvBGR2HSI(Mat& original);
    vector<myNode::Node*> statisticFrequency(const Mat& original,int* arr);
    bool compressionAndEfficiency(int* arr);
    float computComentropy(Mat& input);
    //myNode::Node**  buildShannonFanoNode();
    //int cmp(const void* p1,const void* p2);
public:
    Widget(QWidget *parent = nullptr);

    ~Widget() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
public slots:
        void OnCaptureSignal();
signals:
        void captureSignal();

};
#endif // WIDGET_H
