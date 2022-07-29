#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QCloseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QScrollBar>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QtSql>
#include <QWidget>
#include "MusicList.h"

namespace Ui {
class Widget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;
    
private:
    Ui::Widget *ui;

    //当前播放器
    QMediaPlayer *player;

    //当前播放列表
    QMediaPlaylist *playlist;

    //系统托盘
    QSystemTrayIcon *mySystemTray;
    QAction *systemTray_pre;
    QAction *systemTray_play;
    QAction *systemTray_next;
    QAction *systemTray_playmode;
    QAction *systemTray_quit;

    //歌单
    vector<MusicList> musiclist;

    //“当前播放”列表的右键菜单
    QMenu *menu_playlist;
    //“本地音乐”列表的右键菜单
    QMenu *menu_locallist;
    //“我喜欢”列表的右键菜单
    QMenu *menu_favorlist;
    
    //UI组件额外的一些处理
    void init_UI();
    //初始化一些成员变量以及connect连接
    void init_play();
    //数据库初始化
    void init_sqlite();
    //“本地音乐”、“我喜欢”等歌单的初始化
    void init_musicList();
    //系统托盘初始化
    void init_systemTrayIcon();

    //更新播放信息相关（进度、信息等）
    void updatePosition(qint64 position);
    void setPosition(int position);
    void updateDuration(qint64 duration);
    void updateInfo();
    void updatePlayBtn();

    //响应系统托盘的动作（双击等）
    void systemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason);

    //退出应用
    void quitMusicPlayer();

    //右键菜单项的初始化
    void init_menu();

protected:
    void paintEvent(QPaintEvent *event) override;

    //窗口拖动时记录的起始点
    QPoint offset;

    /*重写Widget的一些方法*/
    //实现窗口可拖动
    void mousePressEvent(QMouseEvent *event) override; 
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //关闭时不退出，而是到系统托盘
    void closeEvent(QCloseEvent *event) override;

private slots:
    //部分右键菜单项对应的操作（即对应QAction连接的槽函数）
    void playlist_removeMusic();//当前播放列表-右键菜单 移除歌曲
    void play_to_favor();//从当前播放添加到我喜欢
    void local_to_favor();//从本地音乐添加到我喜欢
    void local_to_playlist();//从本地音乐添加到当前播放列表
    void favor_to_playlist();//从我喜欢添加到当前播放列表

    //右键列表响应菜单
    void on_playListWidget_customContextMenuRequested(const QPoint &pos);
    void on_localMusicWidget_customContextMenuRequested(const QPoint &pos);
    void on_favorMusicWidget_customContextMenuRequested(const QPoint &pos);

    //切换歌单界面
    void on_btnCurMusic_clicked();
    void on_btnLocalMusic_clicked();
    void on_btnFavorMusic_clicked();

    //退出/最小化
    void on_btnQuit_clicked();    
    void on_btnMin_clicked();
    //关于
    void on_btnAbout_clicked();
    //切换每日一句
    void on_btnSwitch_clicked();

    //播放/暂停/停止/上、下一首
    void on_btnPlay_clicked();
    void on_btnStop_clicked();
    void on_btnPre_clicked();
    void on_btnNext_clicked();

    //播放模式切换（列表循环、单曲循环等）
    void on_btnPlayMode_clicked();

    //添加本地音乐
    void on_btnAdd_clicked();

    //音量键/音量条
    void on_btnVolume_clicked();
    void on_volumeSlider_valueChanged(int value);

    //双击播放列表
    void on_playListWidget_doubleClicked(const QModelIndex &index);
    void on_localMusicWidget_doubleClicked(const QModelIndex &index);
    void on_favorMusicWidget_doubleClicked(const QModelIndex &index);

    //歌单清空歌曲
    void on_btnClear_Current_clicked();
    void on_btnClear_Local_clicked();
    void on_btnClear_Favor_clicked();

    //歌单歌曲排序
    void on_btnSortSinger_Local_clicked();
    void on_btnSortTitle_Local_clicked();
    void on_btnSortSinger_Favor_clicked();
    void on_btnSortTitle_Favor_clicked();
};

#endif // MAINWIDGET_H
