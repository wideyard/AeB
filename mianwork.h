#ifndef MIANWORK_H
#define MIANWORK_H

#include <QWidget>
#include"bomb.h"
#include"player.h"
#include"item.h"
#include"def.h"
#include"envr.h"
#include"enemy.h"
#include"end.h"
//#include<phonon/phonon>
#include<QLabel>
#include<QLCDNumber>
#include<QPushButton>
#include<QTimer>
#include<QSound>

namespace Ui {
class mianwork;
}

class mianwork : public QWidget
{
    Q_OBJECT

public:
    explicit mianwork(QWidget *parent = 0);
    ~mianwork();
    void initGame();                   //游戏设置初始化
    void initWidget(void);             //控件初始化
    void initconnect(void);            //连接初始化
    void layBomb(int master);          //放置炸弹
    void gameOver(int res);            //游戏结束
    void playerMove(void);             //玩家移动
    void bombAbout(void);              //炸弹相关
    void itemAbout();                  //道具相关
    void enemyAbout();                 //敌人相关
    void judgement(void);              //所有判定
    void enemyMove(Enemy * enemy);     //敌人移动
signals:
private slots:
    void process(void);                 //处理函数
    void gameStart(void);               //游戏开始
private:
    QLabel *lifeLabel1,*lifeLabel2;      //显示“XX剩余生命值为：”
    QLCDNumber *lifeNum1,*lifeNum2;     //具体现实生命值
    QLabel *mainView;                   //游戏显示窗口
    QPushButton *startButton;           //开始按键
    QTimer *timer;                      //定时器刷屏
    QSound *mdlay;                      //设雷声音
    QSound *mdBomb;                     //爆炸声音
    QSound *music;                      //播放音乐
    Player *player[2];                  //游戏玩家
    Bomb *head;                         //遍历炸弹链表首
    Item *item;                         //道具
    Plant *plant[PLANTSIZE];            //仙人掌
    Enemy *normalEnemy[ENEMYSIZE];      //敌人
    Enemy *boss[BOSSSIZE];              //boss


    int sthExist[XSIZE][YSIZE];         //特定坐标所在物 0:空 1:道具 2:爆炸的炸弹 3:仙人掌 4:敌人 5:玩家 6:炸弹 7:石头
    Ui::mianwork *ui;
    end *e=new end;


protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
};


#endif // MIANWORK_H



