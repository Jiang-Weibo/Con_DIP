#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include<QWidget>
#include<QPainter>
#include<QPoint>
#include<QPixmap>
#include<QScreen>
#include<QPushButton>
#include<QLabel>
class captureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit captureWidget(QWidget *parent = nullptr,QPixmap* source = nullptr);
private:
    void initWindow();
    void loadBackgroundPixmap();
    QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool m_isMousePress;
    QPixmap m_loadPixmap, m_capturePixmap;
    int m_screenwidth;
    int m_screenheight;
    QPoint m_beginPoint, m_endPoint;
    QPainter m_painter;
    QPushButton* enterButton;
    QLabel* label;
    QPixmap* source;
signals:
    void beginCapture(QPixmap captureImage);
public slots:
    void drawCapture(QPixmap captureImage);
};

#endif // SHOWWIDGET_H
