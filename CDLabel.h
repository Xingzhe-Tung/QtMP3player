#ifndef CDLABEL_H
#define CDLABEL_H
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

//自定义标签,实现CD图片旋转动画
class CDLabel : public QLabel
{
    Q_OBJECT

public:
    CDLabel(QWidget *parent = nullptr);
    ~CDLabel();
    void timerstart();
    void timerstop();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    //定时器
    QTimer  *timer;
    //角度
    int angle;

private slots:
    //定时器超时
    void timerTimeout();
};

#endif // CDLABEL_H
