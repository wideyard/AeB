#include "end.h"
#include "ui_end.h"
#include<Qpainter>
#include<qpainter.h>

end::end(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::end)
{
    ui->setupUi(this);
    this->setGeometry(QRect(900,300,420,614));
}

end::~end()
{
    delete ui;
}

void end::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap(0,0,420,614,QPixmap(":/images/hero3.jpg"));
}
