#ifndef MYQSS_H
#define MYQSS_H
#include<QString>

//播放按钮样式
inline QString PlayStyle()
{
    return "QPushButton"
   " {"
   "background-image:url(:/image/image/image/play.png);"
 "   background-repeat:no-repeat;"
  "  background-position:center center;"
 "   border:none;"
"    }";
}
//暂停按钮样式
inline QString PauseStyle()
{
    return "QPushButton"
   " {"
   "background-image:url(:/image/image/image/pause.png);"
 "   background-repeat:no-repeat;"
  "  background-position:center center;"
 "   border:none;"
"    }";
}

//随机播放按钮格式
inline QString RandomStyle()
{
    return "QPushButton"
   " {"
   "background-image:url(:/image/image/image/random.png);"
 "   background-repeat:no-repeat;"
  "  background-position:center center;"
 "   border:none;"
"    }";
}
//循环播放按钮格式
inline QString LoopStyle()
{
    return "QPushButton"
   " {"
   "background-image:url(:/image/image/image/loop.png);"
 "   background-repeat:no-repeat;"
  "  background-position:center center;"
 "   border:none;"
"    }";
}
//单曲循环按钮格式
inline QString LoopOneStyle()
{
    return "QPushButton"
   " {"
   "background-image:url(:/image/image/image/loop-one.png);"
 "   background-repeat:no-repeat;"
  "  background-position:center center;"
 "   border:none;"
"    }";
}

#endif // MYQSS_H
