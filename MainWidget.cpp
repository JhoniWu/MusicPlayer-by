#include "MainWidget.h"
#include "ui_mainWidget.h"
#include "StyleSheet.h"
#include "Music.h"
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    //UI初始化
    init_UI();
    //初始化生成播放器模块及相关组件
    init_play();
    //右键菜单初始化
    init_menu();
    //数据库初始化
    init_sqlite();
    //歌单初始化
    init_musicList();
    //系统托盘初始化
    init_systemTrayIcon();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::systemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    //双击系统托盘的响应
    case QSystemTrayIcon::DoubleClick:
        //显/隐主界面
        if(isHidden()){
            //如果是主界面隐藏，就显示
            show();
        }else{
            //反之则隐藏
            hide();
        }
        break;
    default:
        break;
    }
}

void MainWidget::quitMusicPlayer()
{
    //退出应用
    QCoreApplication::quit();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    //用QWidget做的主窗体，需要重构paintEvent才能正常在主窗口Widget中显示背景图片
    //https://blog.csdn.net/xiejie0226/article/details/81165379
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    //定义光标位置
    int x = event->pos().x();
    int y = event->pos().y();
    //定义音量条位置
    int x_widget = ui->volumeSlider->geometry().x();
    int y_widget = ui->volumeSlider->geometry().y();
    //定义音量条范围
    int w = ui->volumeSlider->geometry().width();
    int h = ui->volumeSlider->geometry().height();
    //点击界面中音量条外的某点时隐藏音量条
    if (!((x >= x_widget) && (x <= x_widget + w) && (y >= y_widget) && (y <= y_widget+h))){
        ui->volumeSlider->hide();
    }
    
    //记录窗口移动的初始位置
    offset = event->globalPos() - pos();
    //设置事件true
    event->accept();
}
void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    //定义光标位置
    int x = event->pos().x();
    int y = event->pos().y();
    //如果光标位置在titleLabel的高度范围内，在layoutWidget_2的宽度范围内，就可以点击移动实现拖动
    if((y < ui->titleLabel->geometry().height()) && (x < ui->layoutWidget_2->geometry().x())){
        //移动
        move(event->globalPos() - offset);
        //设置事件true
        event->accept();
        //设置鼠标箭头变为握紧手型
        setCursor(Qt::ClosedHandCursor);
    }
}
void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //记录窗口移动的起始位置
    offset = QPoint();
    //设置事件true
    event->accept();
    //设置鼠标握紧手型变为箭头
    setCursor(Qt::ArrowCursor);
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    if(!mySystemTray->isVisible()){
        //如果托盘没有出现就显示
        mySystemTray->show();
    }
    //点击"X"时最小化到托盘
    hide();
    //设置事件false
    event->ignore();
}

void MainWidget::on_btnCurMusic_clicked()
{
    //切换到“当前播放”界面
    ui->Music_List->setCurrentIndex(0);
}
void MainWidget::on_btnLocalMusic_clicked()
{
    //切换到“本地音乐”界面
    ui->Music_List->setCurrentIndex(1);
}
void MainWidget::on_btnFavorMusic_clicked()
{
    //切换到“我喜欢”界面
    ui->Music_List->setCurrentIndex(2);
}

void MainWidget::on_btnQuit_clicked()
{
    //关闭窗口
    close();
}
void MainWidget::on_btnMin_clicked()
{
    //窗口最小化
    showMinimized();
}
void MainWidget::on_btnAbout_clicked()
{
    QMessageBox::about(this,u8"关于","PPMusic\n"
                                   "[快捷操作]\n"
                                   "播放/暂停  -  空格\n"
                                   "上一曲/下一曲  -  Alt键+方向键←/→\n"
                                   "添加本地音乐 - 点击本地音乐界面的添加按钮\n - Ctrl键+O\n"
                                   "[温馨提示]\n"
                                   "鼠标移动到不认识的按钮上，会有相应说明\n"
                                   "[软件简介]\n"
                                   "PPMusic是一款星际穿越(Interstellar)风格的轻量级多功能本地音乐播放器\n"
                                   "命名自planet的首字母，致力于给予用户如星际穿越般的音乐纵享之旅\n"
                                   "[版本信息]\n"
                                   "PPMusic 2.0\n"
                                   "[开发人员]\n"
                                   "Mars\n");
}
void MainWidget::on_btnSwitch_clicked()
{
    srand((unsigned)time(NULL));
    QString text[5]={
        "瑞雪兆丰年，寰宇寻天仙。欲问苍天，是否有仙。待到雪落之时，癫癫癫！",
        "Do not go gentle into that good night.",
        "There is only one god, and his name is death. And there is only one thing we say to death, not today.",
        "昨天是一场梦，明天是黑暗的。",
        "So we beat on, boats against the current, borne back ceaselessly into the past."
    };
    ui->textLabel->setWordWrap(true);
    ui->textLabel->clear();
    ui->textLabel->setText(text[rand()%5]);
    ui->textLabel->show();
}
