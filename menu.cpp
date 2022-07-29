#include "MainWidget.h"
#include "ui_mainWidget.h"

void MainWidget::on_playListWidget_customContextMenuRequested(const QPoint &pos)
{
    //判断光标是否在项目(item)，即歌曲条目上
    if (ui->playListWidget->itemAt(pos) == Q_NULLPTR)
    {
        return;//如果不再就直接return空，结束当前行动
    }
    //在光标位置显示右键菜单
    menu_playlist->exec(QCursor::pos());
}
void MainWidget::on_localMusicWidget_customContextMenuRequested(const QPoint &pos)
{
    //判断光标是否在项目(item)，即歌曲条目上
    if (ui->localMusicWidget->itemAt(pos) == Q_NULLPTR)
    {
        return;//如果不再就直接return空，结束当前行动
    }
    //在光标位置显示右键菜单
    menu_locallist->exec(QCursor::pos());
}
void MainWidget::on_favorMusicWidget_customContextMenuRequested(const QPoint &pos)
{
    //判断光标是否在项目(item)，即歌曲条目上
    if (ui->favorMusicWidget->itemAt(pos) == Q_NULLPTR)
    {
        return;//如果不再就直接return空，结束当前行动
    }
    //在光标位置显示右键菜单
    menu_favorlist->exec(QCursor::pos());
}

void MainWidget::init_menu(){
    //“当前播放”列表右键菜单初始化
    ui->playListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //把移除、打开所在文件夹等行为与有关方法联系起来
    QAction *playlist_delete = new QAction(QIcon(":/image/image/image/remove.png"), u8"移除");
    connect(playlist_delete, &QAction::triggered, this, &MainWidget::playlist_removeMusic);
    QAction *playlist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"), u8"打开所在文件夹");
    connect(playlist_showfile, &QAction::triggered, ui->playListWidget, &MusicListWidget::showInExplorer);
    QAction *playlist_detail = new QAction(QIcon(":/image/image/image/detail.png"), u8"歌曲详情");
    connect(playlist_detail, &QAction::triggered, ui->playListWidget, &MusicListWidget::detail);
    QAction *play_to_favor = new QAction(QIcon(":/image/image/image/To-like.png"), u8"添加到我喜欢");
    connect(play_to_favor, &QAction::triggered, this, &MainWidget::play_to_favor);
    //在菜单中添加行为
    menu_playlist = new QMenu(this);
    menu_playlist->addAction(playlist_delete);
    menu_playlist->addAction(playlist_showfile);
    menu_playlist->addAction(playlist_detail);
    menu_playlist->addAction(play_to_favor);

    //“本地音乐”列表右键菜单初始化
    ui->localMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //把移除、打开所在文件夹等行为与有关方法联系起来
    QAction *locallist_delete = new QAction(QIcon(":/image/image/image/remove.png"), u8"移除");
    connect(locallist_delete, &QAction::triggered, ui->localMusicWidget, &MusicListWidget::removeMusic);
    QAction *locallist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"), u8"打开所在文件夹");
    connect(locallist_showfile, &QAction::triggered, ui->localMusicWidget, &MusicListWidget::showInExplorer);
    QAction *locallist_detail = new QAction(QIcon(":/image/image/image/detail.png"), u8"歌曲详情");
    connect(locallist_detail, &QAction::triggered, ui->localMusicWidget, &MusicListWidget::detail);
    QAction *local_to_favor = new QAction(QIcon(":/image/image/image/To-like.png"), u8"添加到我喜欢");
    connect(local_to_favor, &QAction::triggered, this, &MainWidget::local_to_favor);
    QAction *local_to_playlist = new QAction(QIcon(":/image/image/image/To-playlist.png"), u8"添加到当前播放列表");
    connect(local_to_playlist, &QAction::triggered, this, &MainWidget::local_to_playlist);
    //在菜单中添加行为
    menu_locallist = new QMenu(this);
    menu_locallist->addAction(locallist_delete);
    menu_locallist->addAction(locallist_showfile);
    menu_locallist->addAction(locallist_detail);
    menu_locallist->addAction(local_to_favor);
    menu_locallist->addAction(local_to_playlist);

    //“我喜欢”列表右键菜单初始化
    ui->favorMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //把移除、打开所在文件夹等行为与有关方法联系起来
    QAction *favorlist_delete = new QAction(QIcon(":/image/image/image/remove.png"), u8"移除");
    connect(favorlist_delete, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::removeMusic);
    QAction *favorlist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"), u8"打开所在文件夹");
    connect(favorlist_showfile, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::showInExplorer);
    QAction *favorlist_detail = new QAction(QIcon(":/image/image/image/detail.png"), u8"歌曲详情");
    connect(favorlist_detail, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::detail);
    QAction *favor_to_playlist=new QAction(QIcon(":/image/image/image/To-playlist.png"), u8"添加到当前播放列表");
    connect(favor_to_playlist, &QAction::triggered, this, &MainWidget::favor_to_playlist);
    //在菜单中添加行为
    menu_favorlist = new QMenu(this);
    menu_favorlist->addAction(favorlist_delete);
    menu_favorlist->addAction(favorlist_showfile);
    menu_favorlist->addAction(favorlist_detail);
    menu_favorlist->addAction(favor_to_playlist);
}

void MainWidget::playlist_removeMusic()
{
    //获取“当前播放”列表页中选中的当前行
    int pos = ui->playListWidget->currentRow();
    //获取当前播放列表中正在播放的歌曲的序号
    int playing_pos = playlist->currentIndex();
    //在列表页中移除选中的当前行的音乐
    ui->playListWidget->removeMusic();

    if (pos < playing_pos){
        //如果选中的当前行歌曲排在正在播放的歌曲之前要保持当前播放状态不变
        //记录下删除前的播放状态
        QMediaPlayer::State state = player->state();
        //记录下当前播放的进度
        qint64 position_backup = player->position();
        //让当前正在播放的歌曲序号左移一位，即减一
        playing_pos--;
        //移除当前播放列表中被选中的歌曲
        playlist->removeMedia(pos);
        //设置当前播放列表正在播放的歌曲序号为左移的，即保持同一首歌
        playlist->setCurrentIndex(playing_pos);
        //设置当前播放的歌曲进度为记录时的（删除时的）进度，即保持进度不变
        player->setPosition(position_backup);
        ui->positionSlider->setValue(position_backup);
        //设置当前播放歌曲的播放状态为与删除时一致
        if (state == QMediaPlayer::PlayingState){
            player->play();
        }else if (state == QMediaPlayer::PausedState){
            player->pause();
        }
    }else{
        //如果选中的当前行歌曲排不在正在播放的歌曲之前，那就直接删除即可
        playlist->removeMedia(pos);
    }
}

void MainWidget::play_to_favor()
{
    //记下选择的歌曲当前行
    int pos = ui->playListWidget->currentRow();
    //将选中歌曲添加到“我喜欢”歌单
    ui->favorMusicWidget->musicList.addMusic(ui->playListWidget->musicList.getMusic(pos));
    //更新歌单
    ui->favorMusicWidget->refresh();
}

void MainWidget::local_to_favor()
{
    //记下选择的歌曲当前行
    int pos = ui->localMusicWidget->currentRow();
    //将选中歌曲添加到“我喜欢”歌单
    ui->favorMusicWidget->musicList.addMusic(ui->localMusicWidget->musicList.getMusic(pos));
    //更新歌单
    ui->favorMusicWidget->refresh();
}

void MainWidget::local_to_playlist()
{
    //记下选择的歌曲当前行
    int pos = ui->localMusicWidget->currentRow();
    //创建选中音乐的一个临时音乐
    Music tempMusic = ui->localMusicWidget->musicList.getMusic(pos);
    //在当前播放的列表添加选中音乐
    //因为当前播放列表为临时歌单，只是播放时使用，所以用临时音乐文件
    //在当前播放列表删除时只会删除临时文件，不会删除该本地音乐
    ui->playListWidget->musicList.addMusic(tempMusic);
    //更新歌单
    ui->playListWidget->refresh();
    //在播放器的当前播放列表添加音乐
    playlist->addMedia(tempMusic.getUrl());
}

void MainWidget::favor_to_playlist()
{
    //记下选择的歌曲当前行
    int pos = ui->favorMusicWidget->currentRow();
    //创建选中音乐的一个临时音乐
    Music tempMusic = ui->favorMusicWidget->musicList.getMusic(pos);
    //在当前播放的列表添加选中音乐
    //因为当前播放列表为临时歌单，只是播放时使用，所以用临时音乐文件
    //在当前播放列表删除时只会删除临时文件，不会删除该本地音乐
    ui->playListWidget->musicList.addMusic(tempMusic);
    //更新歌单
    ui->playListWidget->refresh();
    //添加到播放器
    playlist->addMedia(tempMusic.getUrl());
}
