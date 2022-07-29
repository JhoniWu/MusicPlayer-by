#include "Music.h"
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QMessageBox>
#include <QtSql>

Music::Music(QUrl iUrl)
{
    url = iUrl;
    refreshInfo();
}

extern QString formatTime(qint64 timeMilliSeconds);

void Music::refreshInfo()
{
    //创建临时播放器
    QMediaPlayer tempPlayer;
    //设置当前播放器的目标来源（url地址）
    tempPlayer.setMedia(url);
    //https://blog.csdn.net/guanguanboy/article/details/100541982
    //在元数据与播放器对象连接时保持程序进行不卡死，仍能处理别的信息与事件
    while(!tempPlayer.isMetaDataAvailable()){
        QCoreApplication::processEvents();
    }
    //调试时查看有哪些元数据可用
    QStringList list=tempPlayer.availableMetaData();
    //如果有可用的歌曲信息就读取元数据
    if(tempPlayer.isMetaDataAvailable()){
        //读取歌手名等数据（歌手名返回的是QStringList类型的数据）
        //join用,做分隔符合并StringList中的字符串
        author          = tempPlayer.metaData(QStringLiteral("Author")).toStringList().join(",");
        musicName       = tempPlayer.metaData(QStringLiteral("Title")).toString();
        albumTitle      = tempPlayer.metaData(QStringLiteral("AlbumTitle")).toString();
        audioBitRate    = tempPlayer.metaData(QStringLiteral("AudioBitRate")).toInt();
        duration        = tempPlayer.duration();
    }
}

QString Music::getInfo() const
{
    //歌手 - 曲名 [tt:tt]
    return author + " - " + musicName + " [" + formatTime(duration) + "]";
}

void Music::detail()
{
    //初始化QString info的内容
    QString info("歌曲名：%1\n艺术家：%2\n时长：%3\n唱片集：%4\n比特率：%5\n文件路径：%6");
    //用arg给占位的部分赋值
    info = info.arg(musicName, author, formatTime(duration), albumTitle, QString::number(audioBitRate) + "bps", url.toString());
    //显示详情对话框
    QMessageBox::about(Q_NULLPTR, "歌曲信息", info);
}

bool MusicCompare::operator()(const Music &A, const Music &B)
{
    //根据比较方式选择返回值
    switch (key) {
    case MUSICNAME:
        return A.musicName < B.musicName;
    case AUTHOR:
        return A.author < B.author;
    case EQUALITY:
        return A.getUrl() == B.getUrl();
    default:
        return A.getInfo() < B.getInfo();
    }
}
