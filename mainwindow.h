#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVector>
#include <QPushButton>
#include <QLayout>
#include <QListWidget>
#include <QLabel>
#include <QSlider>
#include "CDLabel.h"

//媒体信息结构体
struct MusicItem
{
    QString ItemName;
    QString ItemPath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //音乐播放器
    QMediaPlayer *musicPlayer;

    //音乐播放列表
    QMediaPlaylist *musicPlaylist;

    //媒体信息存储
    QVector<MusicItem> mediaObjectInfo;

    //界面按钮
    QPushButton *pushButton[7];

    //界面容器
    QWidget *vWidget[3];
    QWidget *hWidget[4];

    //界面布局
    QVBoxLayout *vBoxLayout[3];
    QHBoxLayout *hBoxLayout[4];

    //界面其他元素
    QListWidget *ListWidget; //显示播放列表
    CDLabel *cdLabel; //CD图片
    QLabel *Label[4]; //界面标签
    QSlider *DurationBar; //播放进度条

    //界面初始化
    void LayoutInit();

    //扫描歌曲
    void scanSong();

    //媒体播放器类初始化
    void mediaPlayerInit();

private slots:
    //播放按钮点击
    void btn_play_clicked();

    //下一曲按钮点击
    void btn_next_clicked();

    //上一曲按钮点击
    void btn_previous_clicked();

    //媒体状态改变
    void mediaPlayerStateChanged(QMediaPlayer::State);

    //列表单击
    void listWidgetCliked(QListWidgetItem*);

    //媒体列表项改变
    void mediaPlaylistCurrentIndexChanged(int);

    //媒体总长度改变
    void musicPlayerDurationChanged(qint64);

    //媒体播放位置改变
    void mediaPlayerPositionChanged(qint64);

    //播放进度条松开
    void durationSliderReleased();
};

#endif // MAINWINDOW_H
