#include "MainWidget.h"
#include <QApplication>

//输出错误日志
void myMessageOutput(QtMsgType type,const QMessageLogContext &context, const QString &msg)
{
    QString txt;
          switch (type) {
          //调试信息提示
          case QtDebugMsg:
              txt = QString("Debug: %1 from %2,%3,%4").arg(msg).arg(context.file).arg(context.function).arg(context.line);
              break;
          //一般的warning提示
          case QtWarningMsg:
              txt = QString("Warning: %1 from %2,%3,%4").arg(msg).arg(context.file).arg(context.function).arg(context.line);
              break;
          //严重错误提示
          case QtCriticalMsg:
              txt = QString("Critical: %1 from %2,%3,%4").arg(msg).arg(context.file).arg(context.function).arg(context.line);
              break;
          //致命错误提示
          case QtFatalMsg:
              txt = QString("Fatal: %1 from %2,%3,%4").arg(msg).arg(context.file).arg(context.function).arg(context.line);
              abort();
          default:
              break;
          }

    QFile outFile("DebugLog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << endl<< txt << endl;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    
    return a.exec();
}
