#include "widget.h"
#include <QApplication>

#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QFile file(":/qss/loginQSS.qss");
    // 只读方式打开该文件
    file.open(QFile::ReadOnly);
    // 读取文件全部内容，使用tr()函数将其转换为QString类型
    QString styleSheet = QString(file.readAll());
    //QString styleSheet = QLatin1String(file.readAll());   //使用这种方式也可以
    // 为QApplication设置样式表
    qApp->setStyleSheet(styleSheet);
    file.close();

//    w.resize(400, 300);
    w.show();

    return a.exec();
}
