#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>
#include<QMainWindow>
#include<qpainter.h>
#include "mianwork.h"
#include"singlework.h"

namespace Ui {
class figure;
}

class figure : public QWidget
{
    Q_OBJECT

public:
    explicit figure(QWidget *parent = 0);
    ~figure();
    void paintEvent(QPaintEvent *event);


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton1_clicked();

private:
    Ui::figure *ui;
    mianwork *m=new mianwork;
    singlework *s=new singlework;
};

#endif // FIGURE_H
