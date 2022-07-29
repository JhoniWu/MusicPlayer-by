#include "MusicList.h"
#include <QCoreApplication>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QtSql>
#include <algorithm>
#include "MusicListWidget.h"

MusicList::MusicList(const QList<QUrl> &urls, QString iName)
{
    addMusic(urls);
    setName(iName);
}

void MusicList::addMusic(const QList<QUrl> &urls)
{
    //添加一个进度显示对话框
    QProgressDialog proDialog(u8"添加进度",u8"取消",0,urls.size());
    proDialog.setMinimumSize(350,140);
    proDialog.setWindowModality(Qt::WindowModal);
    proDialog.setWindowTitle("正在添加……请稍等");
    proDialog.show();

    int add_progress = 0;
    //对urls遍历，其中每一个QUrl类型的数据都赋给i并进行语句块
    foreach (QUrl i, urls) {
        //每添加好一个，就让进度加一，并更新进度条
        add_progress++;
        proDialog.setValue(add_progress);

        QMimeDatabase db;
        //获取i的格式化为本地文件路径的URL路径
        QMimeType mime = db.mimeTypeForFile(i.toLocalFile());
        //如果文件既不是mpeg格式也不是flag格式就跳过
        if((mime.name() != "audio/mpeg") && (mime.name() != "audio/flac")){
            continue;
        }
        //如果文件格式符合类型就继续
        //在音乐vector的最后面添加Music(i)
        music.push_back(Music(i));
        //如果是本地音乐或者我喜欢，就加入数据库信息
        if (sql_flag){
            music[music.size()-1].insertSQL(listName);
        }
        //如果在加载中点取消就退出
        if (proDialog.wasCanceled())
            break;
    }
    //对歌单列表进行排序
    sort(music.begin(), music.end(), MusicCompare(DEFAULT));
    //使用unique和erase进行去重
    //unique只能把相邻的重复元素放到vector尾部，所以要先排序，并且返回去重后的尾地址
    //在该地址之前的全都是不重复的，而该地址是第一个重复的元素
    //使用erase来把该地址到vector尾的元素全部去除
    //对于unique中判断两首歌曲相同用的是自定义的比较规则MusicCompare进行比较
    music.erase(unique(music.begin(), music.end(), MusicCompare(EQUALITY)), music.end());
    //对该歌单数据库进行更新，相当于refresh_SQL()
    remove_SQL_all();
    insert_SQL_all();
}

void MusicList::addMusic(const Music &iMusic)
{
    //直接在歌曲vector最后添加该音乐
    music.push_back(iMusic);
    //如果是本地音乐或者我喜欢，就加入数据库信息
    if(sql_flag){
        music[music.size()-1].insertSQL(listName);
    }
    //对歌单列表进行排序
    sort(music.begin(), music.end(), MusicCompare(DEFAULT));
    //使用unique和erase进行去重
    //unique只能把相邻的重复元素放到vector尾部，所以要先排序，并且返回去重后的尾地址
    //在该地址之前的全都是不重复的，而该地址是第一个重复的元素
    //使用erase来把该地址到vector尾的元素全部去除
    //对于unique中判断两首歌曲相同用的是自定义的比较规则MusicCompare进行比较
    music.erase(unique(music.begin(), music.end(), MusicCompare(EQUALITY)), music.end());
    //对该歌单数据库进行更新，相当于refresh_SQL()
    remove_SQL_all();
    insert_SQL_all();
}

Music MusicList::getMusic(int pos)
{
    return music[pos];
}

void MusicList::addToPlayList(QMediaPlaylist *playlist)
{
    //遍历储存的音乐，把每一首都加到播放列表中
    for(auto i = music.begin(); i != music.end(); i++){
        playlist->addMedia(i->getUrl());
    }
}

void MusicList::addToListWidget(MusicListWidget *listWidget)
{
    //对music遍历，其中每一个Music类型的数据都赋给i并进行语句块
    foreach(const Music &i, music){
        //新建一个列表的项目
        QListWidgetItem *item = new QListWidgetItem;
        //对列表项目设定图标
        item->setIcon(listWidget->getIcon());
        //对列表项目设定信息内容
        item->setText(i.getInfo());
        //在歌单列表中添加列表项目
        listWidget->addItem(item);
    }
}

void MusicList::remove(int pos){
    //从头开始
    int cur_pos = 0;
    for(auto cur_music = music.begin(); cur_music != music.end(); cur_pos++){
        //如果当前位置就是选中的位置
        if (cur_pos == pos){
            //删除当前位置的音乐并退出
            cur_music = music.erase(cur_music);
            break;
        }else{
            //cur_music往右移一位
            cur_music++;
        }
    }
}
void MusicList::removeMusic(int pos)
{
    //如果是我喜欢或者本地，就同时对数据库操作
    if (sql_flag){
        remove_SQL_all();
        MusicList::remove(pos);
        insert_SQL_all();    
    }else{
        //当前播放就直接删除
        MusicList::remove(pos);
    }
    
}

void MusicList::showInExplorer(int pos)
{
    //获取音乐文件的Url地址
    QString music_Url = music[pos].getUrl().toString();
    //用split以标识符"/"切割字符串来获得所在的文件夹路径
    music_Url.remove(music_Url.split("/").last());
    //打开文件夹
    QDesktopServices::openUrl(music_Url);
}

void MusicList::detail(int pos)
{
    //音乐详情
    music[pos].detail();
}



void MusicList::sort_by(COMPARE key)
{
    //用sort对音乐vector进行排序，从开头到结尾的范围，使用自定义的比较规则MusicCompare进行比较
    sort(music.begin(), music.end(), MusicCompare(key));
    //对该歌单数据库进行更新，相当于refresh_SQL()
    remove_SQL_all();
    insert_SQL_all();
}

void MusicList::clear()
{
    //对音乐vector进行清空
    music.clear();
    //如果需要，即如果是当前播放列表，对数据库进行清理
    if (sql_flag){
        remove_SQL_all();    
    }
}
