#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QMainWindow>
#include<qpainter.h>
#include "figure.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::login *ui;
    figure *f=new figure;
};

#endif // LOGIN_H
