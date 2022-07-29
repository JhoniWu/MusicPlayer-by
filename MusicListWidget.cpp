#include "MusicListWidget.h"

MusicListWidget::MusicListWidget(QWidget *parent)
    :QListWidget (parent)
{
}

void MusicListWidget::refresh()
{
    //先清空列表页面
    clear();
    //把自己的歌单的内容添加到自己（歌单页）上
    musicList.addToListWidget(this);    
}

void MusicListWidget::setMusicList_playing(const MusicList &music_list)
{
    //设定自己的歌单为给定的歌单
    musicList = music_list;
    //设定不能与数据库交互
    musicList.setSQL(false);
    //刷新页面
    refresh();
}

void MusicListWidget::removeMusic()
{
    //在歌单中移除当前选中的音乐
    musicList.removeMusic(currentRow());
    //从歌单列表页中移除
    removeItemWidget(takeItem(currentRow()));
}

void MusicListWidget::showInExplorer()
{
    //在文件夹打开当前行的歌曲
    musicList.showInExplorer(currentRow());
}

void MusicListWidget::detail()
{
    //获取详情
    musicList.detail(currentRow());
}
