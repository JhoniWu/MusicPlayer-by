#include "MainWidget.h"
#include "ui_mainWidget.h"
#include "StyleSheet.h"

void MainWidget::init_UI()
{
    //设置无设定背景的部分透明
    setAttribute(Qt::WA_TranslucentBackground, true);
    //设置无窗口框架边界
    setWindowFlags(Qt::FramelessWindowHint);

    //UI初始化
    //隐藏音量条
    ui->volumeSlider->hide();
    //设置歌单列表中音乐的图标
    ui->playListWidget->setIcon(QIcon(":/image/image/image/music-file.png"));
    ui->localMusicWidget->setIcon(QIcon(":/image/image/image/music-file.png"));
    ui->favorMusicWidget->setIcon(QIcon(":/image/image/image/like.png"));
    //设置每日一句的随机初始化
    MainWidget::on_btnSwitch_clicked();
    //设置动图
    QMovie *movie = new QMovie(":/image/image/image/happyMove.gif");
    ui->happyMove->setMovie(movie);
    movie->start();
}

void MainWidget::init_play()
{
    //播放器初始化
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist;
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);

    //连接槽与信号
    connect(ui->positionSlider, &QAbstractSlider::valueChanged, this, &MainWidget::setPosition);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWidget::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWidget::updateDuration);
    connect(player, &QMediaPlayer::metaDataAvailableChanged, this, &MainWidget::updateInfo);
    connect(player, &QMediaPlayer::stateChanged, this, &MainWidget::updatePlayBtn);
}

void MainWidget::init_sqlite()
{
    //创建数据库
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        //如果已经连接数据库，就使用该数据库
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        //如果没有连接数据库就新建一个数据库建立连接
        database = QSqlDatabase::addDatabase("QSQLITE");
        //定义数据库名称
        database.setDatabaseName("PPMusic.db");
        //定义用户名称
        database.setUserName("PPMusicUser");
        //定义密码
        database.setPassword("PPPPPPP");
        //使用当前连接值打开该数据库连接
        if (!database.open())
        {
            //如果打开不成功就报错并强制退出
            QMessageBox::critical(this,"无法打开数据库文件：PPMusic.db", database.lastError().databaseText());
            exit(-1);
        }
    }
    //https://runebook.dev/zh-CN/docs/qt/qsqlquery
    //检查音乐信息表是否存在，不存在则创建
    //新建一个数据查询表
    QSqlQuery musicQuery;
    //用.exec来执行SQL语句，从sqlite_master选择table类型名字叫MusicInfo的文件
    musicQuery.exec("select count(*) from sqlite_master where type='table' and name='MusicInfo'");
    if (musicQuery.next()){
        //查询执行完毕后,查询的位置在一个invalid记录，并且必须先导航到有效记录，然后才能检索数据值（例如，使用next()）
        if (musicQuery.value(0).toInt()==0){
            //如果表不存在就创建一个表
            QSqlQuery sql_query;
            QString create_sql =
                    "create table MusicInfo (listName varchar(30), url varchar(200), author varchar(50), musicName varchar(50), duration bigint, albumTitle varchar(50), audioBitRate int)";
            //为查询准备
            sql_query.prepare(create_sql);
            //执行先前准备的查询
            sql_query.exec();
        }
    }
}

void MainWidget::init_musicList()
{
    //本地音乐 初始化
    ui->localMusicWidget->musicList.setName("LocalMusic");
    ui->localMusicWidget->musicList.read_fromSQL();
    ui->localMusicWidget->refresh();
    //我喜欢 初始化
    ui->favorMusicWidget->musicList.setName("FavorMusic");
    ui->favorMusicWidget->musicList.read_fromSQL();
    ui->favorMusicWidget->refresh();
}

void MainWidget::init_systemTrayIcon()
{
    //添加系统托盘
    mySystemTray = new QSystemTrayIcon(this);
    //设置系统托盘图标
    mySystemTray->setIcon(QIcon(":/image/image/image/systemTrayIcon.png"));
    //设置系统托盘提示信息
    mySystemTray->setToolTip(u8"PPMusic");
    //关联托盘和托盘功能
    connect(mySystemTray,&QSystemTrayIcon::activated,this,&MainWidget::systemTrayIcon_activated);

    //添加菜单项，关联行为与功能
    systemTray_pre = new QAction(QIcon(":/image/image/image/pre.png"), u8"上一首");
    connect(systemTray_pre, &QAction::triggered, this, &MainWidget::on_btnPre_clicked);
    systemTray_play = new QAction(QIcon(":/image/image/image/play.png"), u8"播放");
    connect(systemTray_play, &QAction::triggered, this, &MainWidget::on_btnPlay_clicked);
    systemTray_next = new QAction(QIcon(":/image/image/image/next.png"), u8"下一首");
    connect(systemTray_next, &QAction::triggered, this, &MainWidget::on_btnNext_clicked);
    systemTray_playmode = new QAction(QIcon(":/image/image/image/loop.png"), u8"循环播放");
    connect(systemTray_playmode, &QAction::triggered, this, &MainWidget::on_btnPlayMode_clicked);
    systemTray_quit = new QAction(QIcon(":/image/image/image/exit.png"), u8"退出应用");
    connect(systemTray_quit, &QAction::triggered, this, &MainWidget::quitMusicPlayer);

    //在菜单添加行为
    QMenu *STContextMenu = new QMenu(this);
    STContextMenu->addAction(systemTray_pre);
    STContextMenu->addAction(systemTray_play);
    STContextMenu->addAction(systemTray_next);
    STContextMenu->addAction(systemTray_playmode);
    STContextMenu->addAction(systemTray_quit);

    //在托盘添加菜单
    mySystemTray->setContextMenu(STContextMenu);
    mySystemTray->show();
}
