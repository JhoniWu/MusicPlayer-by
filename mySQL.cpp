#include "MusicList.h"
#include "Music.h"
#include "MainWidget.h"
#include <QtSql>

void Music::insertSQL(const QString &listName)
{
    //创建查询表
    QSqlQuery sql_query;
    //插入7个占位符的数据，查询准备
    sql_query.prepare("insert into MusicInfo values (?, ?, ?, ?, ?, ?, ?)");
    //将listName值绑定到占位符
    sql_query.addBindValue(listName);
    //将url的String绑定到占位符
    sql_query.addBindValue(url.toString());
    //将author值绑定到占位符
    sql_query.addBindValue(author);
    //将musicName值绑定到占位符
    sql_query.addBindValue(musicName);
    //将duratrion值绑定到占位符
    sql_query.addBindValue(duration);
    //将albumTitle值绑定到占位符
    sql_query.addBindValue(albumTitle);
    //将audioBitRate值绑定到占位符
    sql_query.addBindValue(audioBitRate);
    //对之前准备查询
    //执行SQL语句把数据插入到音乐信息表(MusicInfo Table)中
    //insert into MusicInfo values (listName, url.toString(), author, musicName, duration, albumTitle, audioBitRate)
    sql_query.exec();
}
void MusicList::remove_SQL_all()
{
    //建立一个查询操作
    QSqlQuery sql_query;
    //删除语句，删除全部
    QString delete_sql = "delete from MusicInfo where listName = ?";
    //查询准备
    sql_query.prepare(delete_sql);
    //占位符用listName赋值
    sql_query.addBindValue(listName);
    //执行
    sql_query.exec();
}

void MusicList::insert_SQL_all()
{
    //遍历储存的音乐，全部加到名为listName的歌单数据库中
    for(auto i = music.begin(); i != music.end(); i++){
        //把i的数据加到name中
        i->insertSQL(listName);
    }
}

void MusicList::read_fromSQL()
{
    //建立一个查询操作
    QSqlQuery sql_query;
    //选择语句，从listName歌单中选择各项数据
    QString select_sql = "select url, author, musicName, duration, albumTitle, audioBitRate from MusicInfo where listName = ?";
    //查询准备
    sql_query.prepare(select_sql);
    //占位符用listName赋值
    sql_query.addBindValue(listName);
    //如果正常查询，即歌单存在
    if (sql_query.exec())
    {
        //查询执行完毕后,查询的位置在一个invalid记录，并且必须先导航到有效记录，然后才能检索数据值（例如，使用next()）
        while(sql_query.next())
        {
            //新建一个临时音乐对象
            Music tempMusic;
            //对该音乐赋值
            tempMusic.url           = QUrl(sql_query.value(0).toString());
            tempMusic.author        = sql_query.value(1).toString();
            tempMusic.musicName     = sql_query.value(2).toString();
            tempMusic.duration      = sql_query.value(3).toLongLong();
            tempMusic.albumTitle    = sql_query.value(4).toString();
            tempMusic.audioBitRate  = sql_query.value(5).toInt();
            //在歌单储存的音乐最后加入该音乐
            music.push_back(tempMusic);
        }
    }
}
