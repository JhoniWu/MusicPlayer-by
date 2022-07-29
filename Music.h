#ifndef MUSIC_H
#define MUSIC_H
#include <QString>
#include <QUrl>

//歌曲类
class Music
{
private:
    //歌曲资源地址
    QUrl url;
    //歌手
    QString author;
    //歌曲名
    QString musicName;
    //时长
    qint64 duration;
    //唱片集
    QString albumTitle;
    //比特率
    int audioBitRate;

    //根据歌曲url获得歌曲信息
    void refreshInfo();
    
    friend class MusicCompare;
public:
    Music(){}
    Music(QUrl iUrl);
    
    //返回歌曲Url
    QUrl getUrl() const
    {
        return url;
    }

    //返回歌曲的信息
    QString getInfo() const;

    //弹窗显示歌曲信息
    void detail();

    //存入数据库
    void insertSQL(const QString& listName);
    
    friend class MusicList;
};

//排序的可选属性
enum COMPARE {DEFAULT, MUSICNAME, AUTHOR, EQUALITY};

//用于歌曲排序的函数对象
class MusicCompare
{
    COMPARE key;
public:
    MusicCompare()
    {
        key = DEFAULT;
    }
    MusicCompare(COMPARE iKey)
    {
        key = iKey;
    }
    bool operator()(const Music& A, const Music& B);
};

#endif // MUSIC_H
