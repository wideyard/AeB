#ifndef END_H
#define END_H

#include <QWidget>

namespace Ui {
class end;
}

class end : public QWidget
{
    Q_OBJECT

public:
    explicit end(QWidget *parent = 0);
    ~end();
    void paintEvent(QPaintEvent *event);

private:
    Ui::end *ui;
};

#endif // END_H
