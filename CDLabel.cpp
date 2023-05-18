#include "CDLabel.h"

CDLabel::CDLabel(QWidget *parent) : QLabel(parent)
{
    timer = new QTimer;
    angle = 0;

    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
}

void CDLabel::timerstart()
{
    //设置30ms的倒计时,30ms后timeout
    timer->start(30);
}

void CDLabel::timerstop()
{
    //函数与mediaPlayerStateChanged联系,当state变为停止或暂停时,timer停止倒计时
    //不会触发timeout,也就不会触发timerTimeout的槽函数
    timer->stop();
}

void CDLabel::paintEvent(QPaintEvent *)
{
    //指定父对象
    QPainter painter(this);

    //设置抗锯齿及流畅转换
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //计算旋转角度
    if (angle++ == 360)
        angle = 0;

    //获取setPixmap的pixmap
    QPixmap pix = *CDLabel::pixmap();

    //QRectF 暂时不了解这个类为什么必要
    QRectF rect((this->width() - pix.width())/2,
                (this->height() - pix.height())/2,
                pix.width(),
                pix.height());

    //设置rotate的中心点
    painter.translate(0 + rect.x() + rect.width()/2,
                      0 + rect.y() + rect.height()/2);

    //旋转角度
    painter.rotate(angle);

    //参考点设置回原点的位置
    painter.translate(0 - (rect.x() + rect.width()/2),
                      0 - (rect.y() + rect.height()/2));

    //绘制图形
    painter.drawPixmap(rect , pix, pix.rect());
}

void CDLabel::timerTimeout()
{
    this->update();
}

CDLabel::~CDLabel(){}
