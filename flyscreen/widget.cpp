#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //窗口大小
    resize(LOGIN_WINDOW_WIDTH, LOGIN_WINDOW_HEIGHT);

    setWindowFlags(Qt::Window |
                   Qt::FramelessWindowHint |    //去边框
                   Qt::WindowMinimizeButtonHint);   //点击任务栏图标可最小化
    setAttribute(Qt::WA_TranslucentBackground); //透明背景
    titleWiget = ui->titleWidget;
    inputWiget = ui->inputWidget;
    buttonWiget = ui->buttonWidget;

//图标
    setWindowIcon(QIcon(ICON_IMAGE));
    //标题栏QLabel图标
    ui->iconLabel->setGeometry(0, 0, TITLE_BUTTON_WIDTH, TITLE_BUTTON_HEIGHT);
    QPixmap pix(ICON_IMAGE);
//    ui->iconLabel->setScaledContents(true);
    ui->iconLabel->setPixmap(pix.scaled(ui->iconLabel->size()));

    //标题栏按钮提示
    ui->minToolButton->setToolTip(QString("最小化"));
//    ui->minToolButton->setFixedSize(TITLE_BUTTON_WIDTH, TITLE_BUTTON_HEIGHT);
    ui->minToolButton->setGeometry(this->width()-2*TITLE_BUTTON_WIDTH, 0, TITLE_BUTTON_WIDTH, TITLE_BUTTON_HEIGHT);

    ui->closeToolButton->setToolTip(QString("关闭"));
//    ui->closeToolButton->setFixedSize(TITLE_BUTTON_WIDTH, TITLE_BUTTON_HEIGHT);
    ui->closeToolButton->setGeometry(this->width()-TITLE_BUTTON_WIDTH, 0, TITLE_BUTTON_WIDTH, TITLE_BUTTON_HEIGHT);


    /**********槽连接********************/
    //最小化按钮
    connect(ui->minToolButton, SIGNAL(clicked()), this, SLOT(onclickMinToolButton()));
    //关闭按钮
    connect(ui->closeToolButton,SIGNAL(clicked()), this, SLOT(onclickCloseToolButton()));

/*********输入信息*****************/
    //计算机名
    ui->computerNameEdit->setPlaceholderText("计算机名");
    ui->computerNameEdit->setText(getComputerName());
//    ui->computerNameEdit->setFocusProxy(Qt::NoFocus);   //无法获取鼠标焦点  不可编辑
    ui->computerNameEdit->setEnabled(false);

    //ip
//    ui->ipComboBox->addItems(QStringList(*getIPv4List()));
    foreach(QString ipv4, *getIPv4List()){
        ui->ipComboBox->addItem(ipv4);
    }

    //用户
    ui->userEdit->setPlaceholderText("用户名");


    //密码
    ui->passwordEdit->setPlaceholderText("密码");


/********托盘**************** */
    tray = new QSystemTrayIcon(this);   /* 初始化托盘对象tray */
    tray->setIcon(QIcon(QPixmap(ICON_IMAGE))); /* 设定托盘图标，引号内是自定义的png图片路径 */
    tray->show(); /* 让托盘图标显示在系统托盘上 */
    /* 连接信号与槽，实现单击图标恢复窗口的功能，槽是自定义的槽 */
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(TrayIconAction( QSystemTrayIcon::ActivationReason)));
    /****初始化托盘菜单及功能****/
    menu	= new QMenu(this); /* 初始化菜单 */
    reset	= new QAction(this);  /* 初始化恢复窗口 */
    reset->setText("show");  /* 菜单中的显示窗口，单击连接显示窗口 */
    connect(reset, SIGNAL(triggered()), this, SLOT(showApp()));
    quit = new QAction(this); /* 初始化退出程序 */
    quit->setText("quit");
    connect(quit, SIGNAL(triggered()), this, SLOT(quitApp()));
    tray->setContextMenu(menu);
    menu->addAction(reset);
    menu->addAction(quit);


    /***注册、、登录****************************/
    connect(ui->registerButton, SIGNAL(clicked()), this, SLOT(onclickRegisterButton()));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(onclickLoginButton()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::paintEvent(QPaintEvent *even){
    QPainter painter(this);
    QPixmap pix;
    //背景
    pix.load(LOGIN_BACKGROUND_IMAGE);
//    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    painter.drawPixmap(rect(), pix);

    //窗口 标题 按钮
    ui->minToolButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
//    if(pix.load(MIN_BUTTON_ICON)){
//        ui->minToolButton->setIcon(pix);
//    }
    ui->closeToolButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
//    if(pix.load(CLOSE_BUTTON_ICON)){
//        ui->closeToolButton->setIcon(pix);
//    }
    update();
}


void Widget::mousePressEvent(QMouseEvent *event){
    //鼠标在标题框内
    if(titleWiget->geometry().contains(this->mapFromGlobal(event->globalPos()))){
        mouseInTitle = true;
        mouseMoveStartPoint = event->globalPos();
    }

    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    if(mouseInTitle){
        QPoint movePoint = event->globalPos() - mouseMoveStartPoint;
        mouseMoveStartPoint = event->globalPos();
        this->move(pos().x() + movePoint.x(), pos().y() + movePoint.y());
    }

    QWidget::mouseMoveEvent(event);

}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    mouseInTitle = false;

    QWidget::mouseReleaseEvent(event);
}



/**
 * @brief 点击最小化按钮
 */
void Widget::onclickMinToolButton(){
    showMinimized();
}

/**
 * @brief 点击关闭按钮  隐藏窗口
 */
void Widget::onclickCloseToolButton(){
//    close();
    hide(); //隐藏窗口
}

void Widget::TrayIconAction( QSystemTrayIcon::ActivationReason reason )
/* 参数是Qt的保留字，表示对托盘图标的操作，该槽函数功能只实现了单击功能，可以实现其他功能比如双击、中击，具体可查文档或者Qt自带demo(关键字tray) */
{
    if (reason == QSystemTrayIcon::Trigger )
        this->showNormal();  /* 如果对图标进行单击，则显示正常大小的窗口 */
}

void Widget::showApp(){
    this->showNormal();
}

void Widget::quitApp(){
    close();
}
//获取计算机名
QString Widget::getComputerName(){
    computerName = QHostInfo::localHostName();
    return  computerName;
}

//获取主机的IPv4地址
QList<QString> *Widget::getIPv4List(){
    QList<QString> *ipv4List = new QList<QString>();
    QHostInfo info = QHostInfo::fromName(getComputerName());
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            ipv4List->append(address.toString());
//            qDebug() << address.toString();
        }
    }

    return ipv4List;
}

void Widget::onclickLoginButton(){
    computerName = ui->computerNameEdit->text();
    ipv4 = ui->ipComboBox->currentText();
    username = ui->userEdit->text();
    password = ui->passwordEdit->text();

    qDebug()<<computerName;
    qDebug()<<ipv4;
    qDebug()<<username;
    qDebug()<<password;

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(0, "error", "please check your name or password!");
        return;
    }

    if(!sqlHelper(DB_HOST, "QMYSQL", "root", "123456", "fs", 3306))
    {
        QMessageBox::critical(0, "Database", "connect database error!");
        return;
    }
}

void Widget::onclickRegisterButton(){

}

bool Widget::sqlHelper(QString dbhost, QString dbType, QString dbuser, QString dbpassword, QString dbname, int dbport){
    db = QSqlDatabase::addDatabase(dbType);
    db.setHostName(dbhost);
    db.setUserName(dbuser);
    db.setPassword(dbpassword);
    db.setDatabaseName(dbname);
    db.setPort(dbport);
    if(!db.open()){
        QMessageBox::critical(0, "database", "Database connect error, please check you computer!");
        return false;
    }
    qDebug() <<"数据库连接成功";
    return true;
}
