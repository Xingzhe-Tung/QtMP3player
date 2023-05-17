#include "mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfoList>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0,0,800,480);
    QPalette pal;

    //界面布局的初始化
    LayoutInit();

    //初始化音乐播放器
    mediaPlayerInit();

    //扫描歌曲
    scanSong();

    //按钮信号槽连接
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(btn_previous_clicked()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(btn_play_clicked()));
    connect(pushButton[2], SIGNAL(clicked()),
            this, SLOT(btn_next_clicked()));

    //媒体信号槽连接
    connect(musicPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(musicPlayer,SIGNAL(positionChanged(qint64)),
            this,SLOT(mediaPlayerPositionChanged(qint64)));
    connect(musicPlayer,SIGNAL(durationChanged(qint64)),
            this,SLOT(musicPlayerDurationChanged(qint64)));
    connect(musicPlaylist,SIGNAL(currentIndexChanged(int)),
            this,SLOT(mediaPlaylistCurrentIndexChanged(int)));

    //列表信号槽连接
    connect(ListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(listWidgetCliked(QListWidgetItem*)));

    //Slider信号槽连接
    connect(DurationBar, SIGNAL(sliderReleased()),
            this, SLOT(durationSliderReleased()));

    //重设焦点
    this->setFocus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::LayoutInit()
{
    //按钮
    for(int i = 0; i < 7; i++)
        pushButton[i] = new QPushButton;

    pushButton[0]->setObjectName("btn_presong");
    pushButton[1]->setObjectName("btn_display");
    pushButton[2]->setObjectName("btn_nexsong");
    pushButton[3]->setObjectName("btn_favourite");
    pushButton[4]->setObjectName("btn_mode");
    pushButton[5]->setObjectName("btn_menu");
    pushButton[6]->setObjectName("btn_volume");

    pushButton[1]->setCheckable(true);
    pushButton[3]->setCheckable(true);


    //垂直容器及垂直布局
    for(int i = 0; i < 3; i++)
    {
        vWidget[i] = new QWidget;
        vWidget[i]->setAutoFillBackground(true);
        vBoxLayout[i] = new QVBoxLayout;
    }

    //水平容器及水平布局
    for(int i = 0; i < 4; i++)
    {
        hWidget[i] = new QWidget;
        hWidget[i]->setAutoFillBackground(true);
        hBoxLayout[i] = new QHBoxLayout;
    }

    //标签
    for(int i = 0; i < 4; i++)
        Label[i] = new QLabel;

    //播放列表
    ListWidget = new QListWidget;
    ListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //播放进度条
    DurationBar = new QSlider(Qt::Horizontal);
    DurationBar->setMinimumSize(290,35);
    DurationBar->setMaximumHeight(35);
    DurationBar->setObjectName("durationBar");

    //H0布局
    vWidget[0]->setMinimumSize(310,480);
    vWidget[0]->setMaximumWidth(310);
    vWidget[1]->setMinimumSize(310,480);

    QSpacerItem *hSpacer0 = new
            QSpacerItem(60,480,
                        QSizePolicy::Minimum,QSizePolicy::Minimum);
    QSpacerItem *hSpacer1 = new
            QSpacerItem(60,480,
                        QSizePolicy::Maximum,QSizePolicy::Minimum);
    QSpacerItem *hSpacer2 = new
            QSpacerItem(60,480,
                        QSizePolicy::Maximum,QSizePolicy::Minimum);

    hBoxLayout[0]->addSpacerItem(hSpacer0);
    hBoxLayout[0]->addWidget(vWidget[0]);
    hBoxLayout[0]->addSpacerItem(hSpacer1);
    hBoxLayout[0]->addWidget(vWidget[1]);
    hBoxLayout[0]->addSpacerItem(hSpacer2);
    hBoxLayout[0]->setContentsMargins(0,0,0,0);
    hWidget[0]->setLayout(hBoxLayout[0]);

    this->setCentralWidget(hWidget[0]);

    //v0布局
    Label[0]->setMinimumSize(310,95);
    Label[0]->setMaximumWidth(310);
    QFont font ( "Microsoft YaHei", 9, 75);
    Label[0]->setFont(font);
    Label[0]->setText("正在播放: ");
    hWidget[1]->setMinimumSize(310,80);
    ListWidget->setMinimumSize(310,265);
    ListWidget->setMaximumWidth(310);
    QSpacerItem *vSpacer0 = new
            QSpacerItem(310,10,
                        QSizePolicy::Maximum,QSizePolicy::Minimum);
    QSpacerItem *vSpacer1 = new
            QSpacerItem(310,30,
                        QSizePolicy::Maximum,QSizePolicy::Minimum);

    vBoxLayout[0]->addWidget(Label[0]);
    vBoxLayout[0]->addWidget(ListWidget);
    vBoxLayout[0]->addSpacerItem(vSpacer0);
    vBoxLayout[0]->addWidget(hWidget[1]);
    vBoxLayout[0]->addSpacerItem(vSpacer1);
    vBoxLayout[0]->setContentsMargins(0,0,0,0);

    vWidget[0]->setLayout(vBoxLayout[0]);

    //h1布局
    for(int i = 0; i < 3; i++)
    {
        pushButton[i]->setMinimumSize(80,80);
    }
    QSpacerItem *hSpacer3 = new
            QSpacerItem(35,80,
                        QSizePolicy::Expanding,QSizePolicy::Maximum);
    QSpacerItem *hSpacer4 = new
            QSpacerItem(35,80,
                        QSizePolicy::Expanding,QSizePolicy::Maximum);

    hBoxLayout[1]->addWidget(pushButton[0]);
    hBoxLayout[1]->addSpacerItem(hSpacer3);
    hBoxLayout[1]->addWidget(pushButton[1]);
    hBoxLayout[1]->addSpacerItem(hSpacer4);
    hBoxLayout[1]->addWidget(pushButton[2]);
    hBoxLayout[1]->setContentsMargins(0,0,0,0);
    hWidget[1]->setLayout(hBoxLayout[1]);

    //v1布局
    Label[1]->setMinimumSize(310,310);
    QImage image;
    image.load(":/images/icons/record.png");
    QPixmap pixmap = QPixmap::fromImage(image);
    QPixmap fitpixmap = pixmap.scaled(310,310,
                                      Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    Label[1]->setPixmap(fitpixmap);
    Label[1]->setAlignment(Qt::AlignCenter);
    hWidget[2]->setMinimumSize(310,80);
    hWidget[2]->setMaximumHeight(80);
    QSpacerItem *vSpacer2 = new
            QSpacerItem(310,20,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    QSpacerItem *vSpacer3 = new
            QSpacerItem(310,10,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    QSpacerItem *vSpacer4 = new
            QSpacerItem(310,10,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    QSpacerItem *vSpacer5 = new
            QSpacerItem(310,15,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    vBoxLayout[1]->addSpacerItem(vSpacer2);
    vBoxLayout[1]->addWidget(Label[1]);
    vBoxLayout[1]->addSpacerItem(vSpacer3);
    vBoxLayout[1]->addWidget(DurationBar);
    vBoxLayout[1]->addSpacerItem(vSpacer4);
    vBoxLayout[1]->addWidget(hWidget[2]);
    vBoxLayout[1]->addSpacerItem(vSpacer5);
    vBoxLayout[1]->setContentsMargins(0,0,0,0);

    vWidget[1]->setLayout(vBoxLayout[1]);

    //h2布局
    QSpacerItem *hSpacer5 = new
            QSpacerItem(0,80,
                        QSizePolicy::Minimum,QSizePolicy::Maximum);
    QSpacerItem *hSpacer6 = new
            QSpacerItem(40,80,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    QSpacerItem *hSpacer7 = new
            QSpacerItem(40,80,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    QSpacerItem *hSpacer8 = new
            QSpacerItem(40,80,
                        QSizePolicy::Maximum,QSizePolicy::Maximum);
    QSpacerItem *hSpacer9 = new
            QSpacerItem(0,80,
                        QSizePolicy::Minimum,QSizePolicy::Maximum);
    for (int i = 3; i < 7; i++) {
        pushButton[i]->setMinimumSize(25, 25);
        pushButton[i]->setMaximumSize(25, 25);
    }
    hBoxLayout[2]->addSpacerItem(hSpacer5);
    hBoxLayout[2]->addWidget(pushButton[3]);
    hBoxLayout[2]->addSpacerItem(hSpacer6);
    hBoxLayout[2]->addWidget(pushButton[4]);
    hBoxLayout[2]->addSpacerItem(hSpacer7);
    hBoxLayout[2]->addWidget(pushButton[5]);
    hBoxLayout[2]->addSpacerItem(hSpacer8);
    hBoxLayout[2]->addWidget(pushButton[6]);
    hBoxLayout[2]->addSpacerItem(hSpacer9);
    hBoxLayout[2]->setContentsMargins(0,0,0,0);
    hBoxLayout[2]->setAlignment(Qt::AlignHCenter);

    hWidget[2]->setLayout(hBoxLayout[2]);

}

void MainWindow::scanSong()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/myMusic");

    //转换为绝对路径
    QDir absolutPath = dir.absolutePath();

    //如果文件路径不存在,则创建新路径
    if(absolutPath.exists())
    {
        //创建过滤器
        QStringList filter;
        filter << "*.mp3";

        //获取该目录下的所有文件
        QFileInfoList files =
                absolutPath.entryInfoList(filter,QDir::Files);

        //遍历,将歌曲添加到列表中
        for(int i = 0; i < files.count(); i++)
        {
            //创建音乐媒体结构体对象
            MusicItem info;

            // 使用utf-8编码
            QString fileName = QString::fromUtf8(files.at(i)
                                                 .fileName()
                                                 .replace(".mp3"," ")
                                                 .toUtf8()
                                                 .data());
            info.ItemName = fileName + "\n"
                    +fileName.split("-").at(1);
            info.ItemPath = QString::fromUtf8(files.at(i)
                                              .filePath()
                                              .toUtf8()
                                              .data());
            //添加歌曲到列表中
            if(musicPlaylist->addMedia(QUrl::fromLocalFile(info.ItemPath)))
            {
                //添加到容器数组里存储
                mediaObjectInfo.append(info);
                //添加歌曲名到列表
                ListWidget->addItem(info.ItemName);
            }
            else
            {
                qDebug()<<
                           musicPlaylist->errorString()
                           .toUtf8().data()
                        << endl;
                qDebug()<< "  Error number:"
                         << musicPlaylist->error()
                         << endl;
            }
        }

    }
    else
    {
        dir.mkdir(QCoreApplication::applicationDirPath() + "/myMusic");;
        qDebug()  << "No Music File Exist" << endl;
    }
}

void MainWindow::mediaPlayerInit()
{
    musicPlayer = new QMediaPlayer(this);
    musicPlaylist = new QMediaPlaylist(this);
    /* 确保列表是空的 */
    musicPlaylist->clear();
    /* 设置音乐播放器的列表为mediaPlaylist */
    musicPlayer->setPlaylist(musicPlaylist);
    /* 设置播放模式，Loop是列循环 */
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
}

void MainWindow::btn_play_clicked()
{
    int state = musicPlayer->state();

    switch (state) {
    case QMediaPlayer::StoppedState:
        /* 媒体播放 */
        musicPlayer->play();
        break;

    case QMediaPlayer::PlayingState:
        /* 媒体暂停 */
        musicPlayer->pause();
        break;

    case QMediaPlayer::PausedState:
        musicPlayer->play();
        break;
    }
}

void MainWindow::btn_previous_clicked()
{
    musicPlayer->stop();
    int count = musicPlaylist->mediaCount();
    if (count == 0)
        return ;
    musicPlaylist->previous();
    musicPlayer->play();
}

void MainWindow::btn_next_clicked()
{
    musicPlayer->stop();
    int count = musicPlaylist->mediaCount();
    if (count == 0)
        return ;
    musicPlaylist->next();
    musicPlayer->play();
}

void MainWindow::mediaPlayerStateChanged(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::StoppedState:
        pushButton[1]->setChecked(false);
        break;

    case QMediaPlayer::PlayingState:
        pushButton[1]->setChecked(true);
        break;

    case QMediaPlayer::PausedState:
        pushButton[1]->setChecked(false);
        break;
    }
}

void MainWindow::listWidgetCliked(QListWidgetItem* item)
{
    musicPlayer->stop();
    musicPlaylist->setCurrentIndex(ListWidget->row(item));
    Label[0]->setText("正在播放: " + item->text());
    musicPlayer->play();
}

void MainWindow::mediaPlaylistCurrentIndexChanged(int index)
{
    if (-1 == index)
        return;

    /* 设置列表正在播放的项 */
    ListWidget->setCurrentRow(index);
}

void MainWindow::mediaPlayerPositionChanged(qint64 position)
{
    if (!DurationBar->isSliderDown())
        DurationBar->setValue(position/1000);
}

void MainWindow::musicPlayerDurationChanged(qint64 duration)
{
    DurationBar->setRange(0, duration / 1000);
}

void MainWindow::durationSliderReleased()
{
    /* 设置媒体播放的位置 */
    musicPlayer->setPosition(DurationBar->value() * 1000);
}
