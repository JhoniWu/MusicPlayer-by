#include "MainWidget.h"
#include "ui_mainWidget.h"

void MainWidget::on_playListWidget_doubleClicked(const QModelIndex &index)
{
    //获取当前行
    int i = index.row();
    //设置播放器的当前行为i
    playlist->setCurrentIndex(i);
    //播放
    player->play();
}
void MainWidget::on_localMusicWidget_doubleClicked(const QModelIndex &index)
{
    //清空播放器的当前播放列表
    playlist->clear();
    //把local的所有音乐添加到播放器的当前播放列表(playlist)
    ui->localMusicWidget->musicList.addToPlayList(playlist);
    //把local的所有音乐添加到当前播放列表页的播放列表(current-music)
    ui->playListWidget->setMusicList_playing(ui->localMusicWidget->musicList);
    //获取双击的歌曲的当前行，设置其为当前播放列表当前行并播放
    int i = index.row();
    playlist->setCurrentIndex(i);
    player->play();
    //跳转到当前播放列表
    ui->Music_List->setCurrentIndex(0);
}
void MainWidget::on_favorMusicWidget_doubleClicked(const QModelIndex &index)
{
    //清空当前播放列表
    playlist->clear();
    //把favor的所有音乐添加到播放器的当前播放列表(playlist)
    ui->favorMusicWidget->musicList.addToPlayList(playlist);
    //把favor的所有音乐添加到当前播放列表页的播放列表(current-music)
    ui->playListWidget->setMusicList_playing(ui->favorMusicWidget->musicList);
    //获取双击的歌曲的当前行，设置其为当前播放列表当前行并播放
    int i = index.row();
    playlist->setCurrentIndex(i);
    player->play();
    //跳转到当前播放列表
    ui->Music_List->setCurrentIndex(0);
}

void MainWidget::on_btnClear_Current_clicked()
{
    //定义一个问题对话框，对清空进行确认
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "清空后不可恢复\n确定要清空吗\n", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->playListWidget->musicList.clear();
        ui->playListWidget->refresh();
        //对“当前播放”歌单删除时还要删除播放器的当前播放
        playlist->clear();
    }
}
void MainWidget::on_btnClear_Local_clicked()
{
    //定义一个问题对话框，对清空进行确认
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "清空后不可恢复\n确定要清空吗\n", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->localMusicWidget->musicList.clear();
        ui->localMusicWidget->refresh();
    }
}
void MainWidget::on_btnClear_Favor_clicked()
{
    //定义一个问题对话框，对清空进行确认
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "清空后不可恢复\n确定要清空吗\n", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->favorMusicWidget->musicList.clear();
        ui->favorMusicWidget->refresh();
    }
}

void MainWidget::on_btnSortSinger_Local_clicked()
{
    //按照歌手名排序
    ui->localMusicWidget->musicList.sort_by(AUTHOR);
    ui->localMusicWidget->refresh();
}
void MainWidget::on_btnSortTitle_Local_clicked()
{
    //按照歌曲名排序
    ui->localMusicWidget->musicList.sort_by(MUSICNAME);
    ui->localMusicWidget->refresh();
}
void MainWidget::on_btnSortSinger_Favor_clicked()
{
    //按照歌手名排序
    ui->favorMusicWidget->musicList.sort_by(AUTHOR);
    ui->favorMusicWidget->refresh();
}
void MainWidget::on_btnSortTitle_Favor_clicked()
{
    //按照歌曲名排序
    ui->favorMusicWidget->musicList.sort_by(MUSICNAME);
    ui->favorMusicWidget->refresh();
}
