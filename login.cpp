#include "login.h"
#include "ui_login.h"
#include "figure.h"
#include<Qpainter>
#include<qpainter.h>
#include<QTimer>
#include<QKeyEvent>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setGeometry(QRect(900,300,387,537));
}

login::~login()
{
    delete ui;
}

void login::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap(0,0,387,537,QPixmap(":/images/background.jpg"));
}

void login::on_pushButton_clicked()
{
    this->close();
    f=new figure();
    f->show();
}
