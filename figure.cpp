#include "figure.h"
#include "ui_figure.h"
#include"bombman.h"
#include"mianwork.h"
#include<QApplication>
#include<QMessageBox>
#include<QSplashScreen>
#include<QPixmap>
#include<QDesktopWidget>

figure::figure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::figure)
{
    ui->setupUi(this);
    this->setGeometry(QRect(900,300,552,276));
}

figure::~figure()
{
    delete ui;
}
void figure::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap(0,0,552,276,QPixmap(":/images/figure.jpg"));
}

void figure::on_pushButton_2_clicked()
{
    this->close();
    m=new mianwork();
    m->show();
}

void figure::on_pushButton1_clicked()
{
    this->close();
    s=new singlework();
    s->show();

}
