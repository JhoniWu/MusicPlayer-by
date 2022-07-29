#include "MainWidget.h"
#include "ui_mainWidget.h"
#include "StyleSheet.h"

QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;   //1000ms = 1s
    const qint64 minutes = seconds / 60;        //60s = 1min
    seconds -= minutes * 60;                    //除去分钟后剩下的秒
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
    //规范时间格式    return minutes:seconds;
    //.arg(赋值minutes/seconds, 字符域宽度2(最多为00:00), 10进制数, 用0补空位)
    //QString初始化教程(https://developer.aliyun.com/article/805005)
}

void MainWidget::updatePosition(qint64 position)
{
    //更新进度条
    ui->positionSlider->setValue(static_cast<int>(position));
    //更新进度时间(当前时间/歌曲时长)
    ui->positionLabel->setText(formatTime(position) + "/" + formatTime(player->duration()));
}

void MainWidget::setPosition(int position)
{
    //设置播放进度
    player->setPosition(position);
}

void MainWidget::updateDuration(qint64 duration)
{
    //设置进度条范围
    ui->positionSlider->setRange(0, static_cast<int>(duration));
    //设置进度条状态，只有正在播放(即读取出的当前音乐时长大于0)时才可操作，即true;
    ui->positionSlider->setEnabled(static_cast<int>(duration) > 0);
    if (!(static_cast<int>(duration) > 0)) {
        //定义无音乐播放时的界面
        mySystemTray->setToolTip(u8"PPMusic");
        QImage image(":/image/image/image/non-music.png");
        ui->nonMusic_Label->setPixmap(QPixmap::fromImage(image));
        ui->musicInformationLabel->setText("");
    }
    //设置进度条步长，即从开头到结尾走多少步
    ui->positionSlider->setPageStep(static_cast<int>(duration) / 10);
}

void MainWidget::updateInfo()
{
    if (player->isMetaDataAvailable()) {
        //返回可用MP3元数据列表（调试时可以查看）(https://www.zhihu.com/question/36859497)
        QStringList listInfo_debug = player->availableMetaData();
        //歌曲信息
        QString author = player->metaData(QStringLiteral("Author")).toStringList().join(",");
        QString musicName = player->metaData(QStringLiteral("Title")).toString();
        QString albumTitle = player->metaData(QStringLiteral("AlbumTitle")).toString();

        //更新系统托盘图标信息显示
        QString info = "";
        info.append(author);
        info.append(" - " + musicName);
        info.append(" [" + formatTime(player->duration()) + "]");
        mySystemTray->setToolTip("正在播放：" + info);

        //寻找封面图片
        QImage picImage = player->metaData(QStringLiteral("ThumbnailImage")).value<QImage>();
        //如果找不到就使用原图不更换
        if (picImage.isNull())
            picImage = QImage(":/image/image/image/non-music.png");
        ui->nonMusic_Label->setPixmap(QPixmap::fromImage(picImage));
        //图片自适应框体控件大小
        ui->nonMusic_Label->setScaledContents(true);

        //遍历当前播放列表的歌曲，更改音乐图标为“当前播放列表”的图标
        for(int i = 0; i < playlist->mediaCount(); i++){
            QListWidgetItem *p = ui->playListWidget->item(i);
            p->setIcon(ui->playListWidget->getIcon());
        }

        //改变正在播放歌曲的图标
        QListWidgetItem *p = ui->playListWidget->item(playlist->currentIndex());
        p->setIcon(QIcon(":/image/image/image/music-playing.png"));

        //在进度条上方显示歌曲信息
        ui->musicInformationLabel->setText(u8"歌曲名：" + musicName + u8"   歌手：" + author+ u8"   专辑：" + albumTitle);
    }
}

void MainWidget::updatePlayBtn()
{
    if (player->state() == QMediaPlayer::PlayingState)
    {
        //如果正在播放，界面使用播放格式，系统托盘的文本显示暂停，表示点击将会暂停，而图标是播放的图标，表示正在播放
        ui->btnPlay->setStyleSheet(PlayStyle());
        systemTray_play->setIcon(QIcon(":/image/image/image/pause.png"));
        systemTray_play->setText(u8"暂停");
    }else{
        //如果其他则反之
        ui->btnPlay->setStyleSheet(PauseStyle());
        systemTray_play->setIcon(QIcon(":/image/image/image/play.png"));
        systemTray_play->setText(u8"播放");
    }
}
