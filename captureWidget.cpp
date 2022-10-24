#include "captureWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include<QDebug>
captureWidget::captureWidget(QWidget *parent,QPixmap* source) : QWidget(parent), m_isMousePress(false)
{
    this->source = source;
    label = new QLabel(this);
    connect(this,&captureWidget::beginCapture,this,&captureWidget::drawCapture);
    initWindow();
    loadBackgroundPixmap();
}

void captureWidget::initWindow()
{
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
}

void captureWidget::loadBackgroundPixmap()
{
    qDebug()<<"123";
    //QScreen qs();
    m_loadPixmap = QPixmap::grabWindow(QApplication::desktop()->winId()); //抓取当前屏幕的图片;
    qDebug()<<"456";
    m_screenwidth = m_loadPixmap.width();
    m_screenheight = m_loadPixmap.height();
}

void captureWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isMousePress = true;
        m_beginPoint = event->pos();
    }

    return QWidget::mousePressEvent(event);
}

void captureWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isMousePress)
    {
        m_endPoint = event->pos();
        update();
    }
    return QWidget::mouseMoveEvent(event);
}

void captureWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_endPoint = event->pos();
    m_isMousePress = false;
    return QWidget::mouseReleaseEvent(event);
}

void captureWidget::paintEvent(QPaintEvent *event)
{
    m_painter.begin(this);          //进行重绘;

    QColor shadowColor = QColor(0, 0, 0, 100);                      //阴影颜色设置;
    m_painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));    //设置画笔;
    m_painter.drawPixmap(0, 0, m_loadPixmap);                       //将背景图片画到窗体上;
    m_painter.fillRect(m_loadPixmap.rect(), shadowColor);           //画影罩效果;

    if (m_isMousePress)
    {
        QRect selectedRect = getRect(m_beginPoint, m_endPoint);
        m_capturePixmap = m_loadPixmap.copy(selectedRect);// 将m_loadPixmap的selectedRect部分copy下来 传给m_capturePixmap
        m_painter.drawPixmap(selectedRect.topLeft(), m_capturePixmap);//  将selectedRect.topLeft()作为原点开始画m_capturePixmap这张图
        m_painter.drawRect(selectedRect);// 在selectedRect这个位置画一个Rect
    }


    m_painter.end();  //重绘结束;
}

void captureWidget::drawCapture(QPixmap captureImage)
{
    label->setPixmap(captureImage);
    *(this->source) = QPixmap(captureImage);
    captureImage.save("F:\\JNU\\DigitalImageProcess\\DigitalImageProcessWork\\3.bmp");

}

void captureWidget::keyPressEvent(QKeyEvent *event)
{
    // Esc 键退出截图;
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
    // Eeter键完成截图;
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        //emit beginCapture(m_capturePixmap);
        drawCapture(m_capturePixmap);
        close();
    }
}

QRect captureWidget::getRect(const QPoint &beginPoint, const QPoint &endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    // 避免宽或高为零时拷贝截图有误;
    // 可以看QQ截图，当选取截图宽或高为零时默认为2;
    if (selectedRect.width() == 0)
    {
        selectedRect.setWidth(1);
    }
    if (selectedRect.height() == 0)
    {
        selectedRect.setHeight(1);
    }

    return selectedRect;
}
