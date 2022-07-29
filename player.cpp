#include "MainWidget.h"
#include "ui_mainWidget.h"
#include "StyleSheet.h"

void MainWidget::on_btnPlay_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState)
     {
        player->pause();
     }
    else if (player->state() == QMediaPlayer::PausedState){

        player->play();
    }
    else if ((!playlist->isEmpty()) && ((player->state() == QMediaPlayer::StoppedState)))
    {
        playlist->setCurrentIndex(0);
        player->play();
    }
}
void MainWidget::on_btnStop_clicked()
{
    player->stop();
}
void MainWidget::on_btnPre_clicked()
{
    playlist->previous();
}
void MainWidget::on_btnNext_clicked()
{
    playlist->next();
}

void MainWidget::on_btnPlayMode_clicked()
{
    //播放模式切换顺序为：循环播放-->随机播放-->单曲循环
    if (playlist->playbackMode() == QMediaPlaylist::Loop){
        //更改图标样式及提示文字
        ui->btnPlayMode->setStyleSheet(RandomStyle());
        ui->btnPlayMode->setToolTip(u8"随机播放");
        //更改托盘样式及提示文字
        systemTray_playmode->setIcon(QIcon(":/image/image/image/random.png"));
        systemTray_playmode->setText(u8"随机播放");
        //更改播放器模式
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else if (playlist->playbackMode() == QMediaPlaylist::Random){
        //更改图标样式及提示文字
        ui->btnPlayMode->setStyleSheet(LoopOneStyle());
        ui->btnPlayMode->setToolTip(u8"单曲循环");
        //更改托盘样式及提示文字
        systemTray_playmode->setIcon(QIcon(":/image/image/image/loop-one.png"));
        systemTray_playmode->setText(u8"单曲循环");
        //更改播放器模式
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if (playlist->playbackMode() == QMediaPlaylist::CurrentItemInLoop){
        //更改图标样式及提示文字
        ui->btnPlayMode->setStyleSheet(LoopStyle());
        ui->btnPlayMode->setToolTip(u8"循环播放");
        //更改托盘样式及提示文字
        systemTray_playmode->setIcon(QIcon(":/image/image/image/loop.png"));
        systemTray_playmode->setText(u8"循环播放");
        //更改播放器模式
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void MainWidget::on_btnAdd_clicked()
{
        QFileDialog fileDialog(this);
        QStringList filters;

        //设置为打开文件对话框
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        //点击OK后对话框返回存在的单个文件名
        fileDialog.setFileMode(QFileDialog::ExistingFiles);
        //设置窗口标题
        fileDialog.setWindowTitle(tr("添加本地音乐"));
        //设置默认后缀
        fileDialog.setDefaultSuffix("mp3");

        //设置文件过滤器
        filters << "Music files (*.mp3 *.wav *.wma *.flac)"
                << "Any files (*)";
        //过滤
        fileDialog.setNameFilters(filters);
        //设置打开对话框后显示的目录(本机的音乐文件夹目录，用户主目录下的音乐文件夹)
        fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
        if (fileDialog.exec() == QDialog::Accepted){
           QList<QUrl> urls = fileDialog.selectedUrls();
           ui->localMusicWidget->musicList.addMusic(urls);
           ui->localMusicWidget->refresh();
           ui->Music_List->setCurrentIndex(1);//切换到本地音乐
        }

}

void MainWidget::on_btnVolume_clicked()
{
    //点击音量按钮时如果音量条是显示的就隐藏，如果是隐藏的就显示
    if (ui->volumeSlider->isHidden()){
        ui->volumeSlider->show();
    }else{
        ui->volumeSlider->hide();
    }
}
void MainWidget::on_volumeSlider_valueChanged(int value)
{
    //拖动音量条来控制音量
    player->setVolume(value);
}
