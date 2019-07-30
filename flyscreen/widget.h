#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include<QStyle>
#include <QPoint>
#include <QMouseEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QAction>
#include <QHostInfo>
#include <QList>
#include<QStringList>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>


#include "self_define.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *even);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QString getComputerName();  //获取计算机名
    QList<QString> *getIPv4List();    //获取主机的IPv4地址

public slots:
    void showApp();
    void quitApp();
    void onclickMinToolButton();    //点击最小化按钮
    void onclickCloseToolButton();  //点击关闭按钮

    void onclickRegisterButton();       //注册
    void onclickLoginButton();      //登录

    void TrayIconAction(QSystemTrayIcon::ActivationReason reason);//对托盘图标操作的槽

private:
    bool sqlHelper(QString dbhost, QString dbType,  QString dbuser, QString dbpassword, QString dbname, int dbport);

private:
    Ui::Widget *ui;

    bool mouseInTitle = false;      //鼠标在标题栏
    QString computerName;   //计算机名
    QString ipv4;   //ipv4
    QString username;   //用户名
    QString password;   //密码

    QSqlDatabase db;    //数据库


    QWidget *titleWiget;
    QWidget *inputWiget;
    QWidget *buttonWiget;

    QPoint mouseMoveStartPoint; //鼠标移动事件 起始坐标点

    QSystemTrayIcon *tray;//托盘图标
    QMenu *menu;//托盘菜单
    QAction *reset;//菜单实现功能：恢复窗口
    QAction *quit;//菜单实现功能：退出程序
};

#endif // WIDGET_H
