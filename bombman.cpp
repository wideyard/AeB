#include"bombman.h"
#include"def.h"
#include<QWidget>
#include<QLabel>
#include<QLCDNumber>
#include<QPushButton>
#include<QTimer>
#include<QKeyEvent>
#include<QMessageBox>
#include<QPainter>
#include<QRect>
#include<QImage>
#include<QString>
#include<QBrush>
#include<QPen>
#include<QtCore>
#include<QTime>
#include<QSound>

BombMan::BombMan(QWidget *parent)
    :QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);            //同时响应tab与键盘事件
    setFixedSize(470,530);
    //setFixedSize(920,980);
    initGame();
    initWidget();
    initconnect();
}

void BombMan::initGame()
{
    int i,j;
    qsrand(QTime::currentTime().msec());
    for(i=0;i<450/BLOCK;i++)
    {
        for(j=0;j<450/BLOCK;j++)
        {
            sthExist[i][j]=0;           //特定坐标所在物 0:空 1:道具 2:爆炸的炸弹 3:仙人掌 4:敌人 5:玩家 6:炸弹 7:石头
        }
    }
//    for(i=0;i<900/BLOCK;i++)
//    {
//        for(j=0;j<900/BLOCK;j++)
//        {
//            sthExist[i][j]=0;
//        }
//    }
    player[0]=new Player(0,0);
    sthExist[0][0]=5;
    player[1]=new Player(14,14);
    sthExist[14][14]=5;
    head=new Bomb(0,0,2);
    item=new Item(-1,3000/FRAME);                   //伪道具
    for(i=0;i<STONESIZE;i++)                        //摆放岩石
    {
        sthExist[stone[i][0]][stone[i][1]]=7;
    }
    for(i=0;i<PLANTSIZE;i++)                                //摆放仙人掌
    {
        int x,y;
        do
        {
            x=qrand()%15;
            y=qrand()%15;
        }
        while(sthExist[x][y]!=0);
        plant[i]=new Plant(x,y);
        sthExist[x][y]=3;
    }
    for(i=0;i<ENEMYSIZE;i++)
    {
        int x,y;
        do
        {
            x=qrand()%15;
            y=qrand()%15;
        }
        while(sthExist[x][y]!=0);
        normalEnemy[i]=new Enemy(x,y,0,500/FRAME);
        sthExist[x][y]=4;
    }
    for(i=0;i<BOSSSIZE;i++)
    {
        int x,y;
        do
        {
            x=qrand()%15;
            y=qrand()%15;
        }
        while(sthExist[x][y]!=0);
        boss[i]=new Enemy(x,y,1,400/FRAME);
        sthExist[x][y]=4;
    }
}

void BombMan::initWidget(void)
{
    lifeLabel1=new QLabel(this);
    lifeLabel1->setText("1P HP:");
    lifeLabel1->setGeometry(10,5,50,30);

    lifeNum1=new QLCDNumber(this);
    lifeNum1->setSegmentStyle(QLCDNumber::Flat);
    lifeNum1->setGeometry(65,5,120,30);

    lifeLabel2=new QLabel(this);
    lifeLabel2->setText("2P HP:");
    lifeLabel2->setGeometry(285,5,50,30);
    //lifeLabel2->setGeometry(735,5,50,30);

    lifeNum2=new QLCDNumber(this);
    lifeNum1->setSegmentStyle(QLCDNumber::Flat);
    lifeNum2->setGeometry(340,5,120,30);
    //lifeNum2->setGeometry(790,5,120,30);

    mainView=new QLabel(this);
    mainView->setGeometry(10,40,450,450);
    //mainView->setGeometry(10,40,900,900);
    mainView->setLineWidth(2);
    mainView->setFrameStyle(QFrame::Box |QFrame::Raised);

    startButton=new QPushButton(this);
    startButton->setText("&START");
    startButton->setGeometry(190,495,90,30);
    //startButton->setGeometry(400,945,90,30);

    timer=new QTimer;
    timer->stop();
    mdlay=new QSound(":/sound/lay.wav");
    mdBomb=new QSound(":/sound/bomb.WAV");
}
void BombMan::initconnect(void)
{
    connect(timer,SIGNAL(timeout()),this,SLOT(process()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(gameStart()));
}
void BombMan::gameStart(void)
{
    lifeNum1->display(player[0]->restLife);
    lifeNum2->display(player[1]->restLife);
    if(!timer->isActive())
        timer->start(FRAME);
    update();
}
void BombMan::process(void)
{
    playerMove();
    bombAbout();
    itemAbout();
    enemyAbout();
    judgement();
    update();
}
void BombMan::keyPressEvent(QKeyEvent * event)
{
    if(timer->isActive())
    {
        switch(event->key())
        {
            case Qt::Key_W:
                            player[0]->direction=4;
                            break;
            case Qt::Key_D:
                            player[0]->direction=3;
                            break;
            case Qt::Key_S:
                            player[0]->direction=2;
                            break;
            case Qt::Key_A:
                            player[0]->direction=1;
                            break;
            case Qt::Key_J:
                            layBomb(0);
                            break;
            case Qt::Key_Up:
                            player[1]->direction=4;
                            break;
            case Qt::Key_Right:
                            player[1]->direction=3;
                            break;
            case Qt::Key_Down:
                            player[1]->direction=2;
                            break;
            case Qt::Key_Left:
                            player[1]->direction=1;
                            break;
            case Qt::Key_0:
                            layBomb(1);
                            break;
            case Qt::Key_Space:
                            break;
            default:
                            player[0]->direction=player[0]->direction;
                            player[1]->direction=player[1]->direction;
        }
    }
}
void BombMan::layBomb(int master)
{
        if((player[master]->bombCount) < (player[master]->bombCountLimit))
        {          
            mdlay->play();
            Bomb *p=head;
            while(p->next!=0)
            {
                p=p->next;
            }
            p->next=new Bomb(player[master]->x,player[master]->y,master);
            player[master]->bombCount=player[master]->bombCount+1;
            sthExist[player[master]->x][player[master]->y]=6;
        }
}
void BombMan::gameOver(int res)
{
    if(res==0)
    {
        QMessageBox::about(this,"Resault","player1 win the game!");
    }
    else
    {
        QMessageBox::about(this,"Resault","Player2 win the game!");
    }
    timer->stop();
    free(player[0]);
    free(player[1]);
    Bomb *p=head;
    while(p)
    {
        p=p->next;
        free(head);
        head=p;
    }
    for(int i=0;i<PLANTSIZE;i++)
    {
        free(plant[i]);
    }
    for(int i=0;i<ENEMYSIZE;i++)
    {
        free(normalEnemy[i]);
    }
    for(int i=0;i<BOSSSIZE;i++)
    {
        free(boss[i]);
    }
    initGame();
}
void BombMan::paintEvent(QPaintEvent *)
{
    QPainter painter(this);                   //铺白
    QRect rect=QRect(10,40,450,450);
    //QRect rect=QRect(10,40,900,900);
    painter.fillRect(rect,Qt::white);
    QImage itemImg;                           //道具
    QString itmImgStr[3]={":/images/i001.jpg",":/images/i002.jpg",":/images/i003.jpg"};
    if(item->id >= 0)
    {
        if(sthExist[item->x][item->y]==1)
        {
            itemImg.load(itmImgStr[item->id]);
            painter.drawImage(10+BLOCK*(item->x),40+BLOCK*(item->y),itemImg);
        }
    }
    QImage stoneImg[4];                                                             //石头 为4张图片
    QString stoneStr[4]={":/images/stone1.jpg",":/images/stone2.jpg",":/images/stone3.jpg",":/images/stone4.jpg"};
    for(int i=0;i<4;i++)
    {
        stoneImg[i].load(stoneStr[i]);
    }
    for(int i=0;i<STONESIZE;i++)
    {
        painter.drawImage(10+BLOCK*stone[i][0],40+BLOCK*stone[i][1],stoneImg[i%4]);
    }

    QImage enemyImg,bossImg;                                                        //敌人，包括普通和boss
    QString enemyStr=":/images/enemy2.jpg";
    QString bossStr=":/images/enemy1.jpg";
    enemyImg.load(enemyStr);
    bossImg.load(bossStr);
    for(int i=0;i<ENEMYSIZE;i++)
    {
        if(sthExist[normalEnemy[i]->x][normalEnemy[i]->y]==4)
        {
            painter.drawImage(10+BLOCK*(normalEnemy[i]->x),40+BLOCK*(normalEnemy[i]->y),enemyImg);
        }
    }
    for(int i=0;i<BOSSSIZE;i++)
    {
        if(sthExist[boss[i]->x][boss[i]->y]==4)
        {
            painter.drawImage(10+BLOCK*(boss[i]->x),40+BLOCK*(boss[i]->y),bossImg);
        }
    }

    QImage plantImg;                                                              //仙人掌
    QString plantStr=":/images/plant.jpg";
    plantImg.load(plantStr);
    for(int i=0;i<PLANTSIZE;i++)
    {
        if(sthExist[plant[i]->x][plant[i]->y]==3)
        {
            painter.drawImage(10+BLOCK*(plant[i]->x),40+BLOCK*(plant[i]->y),plantImg);
        }
    }
                                              //载人
    QImage image[2];
    QString imgStr[2][4]={{":/images/p02.gif",":/images/p01.gif",":/images/p03.gif",":/images/p04.gif"},
                          {":/images/p12.gif",":/images/p11.gif",":/images/p13.gif",":/images/p14.gif"}};
    int x[2];
    int y[2];
    x[0]=10+BLOCK*(player[0]->x);
    y[0]=40+BLOCK*(player[0]->y);
    x[1]=10+BLOCK*(player[1]->x);
    y[1]=40+BLOCK*(player[1]->y);
    for(int i=0;i<2;i++)
    {
        if(player[i]->direction > 0)
        {
            player[i]->defDir=player[i]->direction-1;
            image[i].load(imgStr[i][player[i]->defDir]);
            painter.drawImage(x[i],y[i],image[i]);
            player[i]->direction=0;
        }
        else
        {
            image[i].load(imgStr[i][player[i]->defDir]);
            painter.drawImage(x[i],y[i],image[i]);
        }
    }
    Bomb *p=head->next;                              //炸弹
    while(p)
    {
        int t=((p->lastTime)*FRAME)/100;         //化为以0.1秒为单位
        if( t > 10 )
        {
            painter.setBrush(Qt::black);
            painter.setPen(Qt::black);
            if((t%10)>5)
            {
                painter.drawEllipse((p->x)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
            }
            else
            {
                painter.drawEllipse((p->x)*BLOCK+10,((p->y)*BLOCK+40),BLOCK*2/3,BLOCK*2/3);
            }
        }
        else
        {
            painter.setBrush(Qt::red);
            painter.setPen(Qt::red);
            if(t>8)
            {
                painter.drawRect((p->x)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 0)&&(sthExist[p->x-1][p->y]<7))
                    painter.drawRect(((p->x)-1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 14)&&(sthExist[(p->x)+1][p->y]<7))
                    painter.drawRect(((p->x)+1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 0)&&(sthExist[p->x][p->y-1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-1)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 14)&&(sthExist[p->x][p->y+1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+1)*BLOCK+40),BLOCK,BLOCK);
            }
            else if(t>7)
            {
                painter.drawRect((p->x)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 0)&&(sthExist[p->x-1][p->y]<7))
                    painter.drawRect(((p->x)-1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 14)&&(sthExist[(p->x)+1][p->y]<7))
                    painter.drawRect(((p->x)+1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 0)&&(sthExist[p->x][p->y-1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-1)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 14)&&(sthExist[p->x][p->y+1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+1)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 1)&&(sthExist[p->x-2][p->y]<7))
                    painter.drawRect(((p->x)-2)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 13)&&(sthExist[(p->x)+2][p->y]<7))
                    painter.drawRect(((p->x)+2)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 1)&&(sthExist[p->x][p->y-2]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-2)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 13)&&(sthExist[p->x][p->y+2]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+2)*BLOCK+40),BLOCK,BLOCK);
            }
            else if((player[p->master]->atackRange) > 2)
            {
                painter.drawRect((p->x)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 0)&&(sthExist[p->x-1][p->y]<7))
                    painter.drawRect(((p->x)-1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 14)&&(sthExist[(p->x)+1][p->y]<7))
                    painter.drawRect(((p->x)+1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 0)&&(sthExist[p->x][p->y-1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-1)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 14)&&(sthExist[p->x][p->y+1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+1)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 1)&&(sthExist[p->x-2][p->y]<7))
                    painter.drawRect(((p->x)-2)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 13)&&(sthExist[(p->x)+2][p->y]<7))
                    painter.drawRect(((p->x)+2)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 1)&&(sthExist[p->x][p->y-2]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-2)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 13)&&(sthExist[p->x][p->y+2]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+2)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 2)&&(sthExist[p->x-3][p->y]<7))
                    painter.drawRect(((p->x)-3)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 12)&&(sthExist[(p->x)+3][p->y]<7))
                    painter.drawRect(((p->x)+3)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 2)&&(sthExist[p->x][p->y-3]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-3)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 12)&&(sthExist[p->x][p->y+3]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+3)*BLOCK+40),BLOCK,BLOCK);
            }
            else
            {
                painter.drawRect((p->x)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 0)&&(sthExist[p->x-1][p->y]<7))
                    painter.drawRect(((p->x)-1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 14)&&(sthExist[(p->x)+1][p->y]<7))
                    painter.drawRect(((p->x)+1)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 0)&&(sthExist[p->x][p->y-1]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-1)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 14)&&sthExist[p->x][p->y+1]<7)
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+1)*BLOCK+40),BLOCK,BLOCK);
                if((p->x > 1)&&(sthExist[p->x-2][p->y]<7))
                    painter.drawRect(((p->x)-2)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->x < 13)&&(sthExist[(p->x)+2][p->y]<7))
                    painter.drawRect(((p->x)+2)*BLOCK+10,((p->y)*BLOCK+40),BLOCK,BLOCK);
                if((p->y > 1)&&(sthExist[p->x][p->y-2]<7))
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)-2)*BLOCK+40),BLOCK,BLOCK);
                if((p->y < 13)&&sthExist[p->x][p->y+2]<7)
                    painter.drawRect((p->x)*BLOCK+10,(((p->y)+2)*BLOCK+40),BLOCK,BLOCK);
            }
        }
        p=p->next;
    }

}
void BombMan::playerMove(void)
{
    for(int i=0;i<2;i++)
    {
        switch(player[i]->direction)
        {
            case 1:
                if(player[i]->x > 0)
                {
                    if(sthExist[(player[i]->x)-1][player[i]->y]<=5)
                    {
                        if(sthExist[(player[i]->x)-1][player[i]->y]==0)
                        {
                            sthExist[(player[i]->x)-1][player[i]->y]=5;
                        }
                        if(sthExist[player[i]->x][player[i]->y]==5)
                        {
                            sthExist[player[i]->x][player[i]->y]=0;
                        }
                        player[i]->x=player[i]->x-1;
                    }
                }
                break;
            case 2:
                if(player[i]->y < 14)
                {

                    if(sthExist[player[i]->x][(player[i]->y)+1]<=5)
                    {
                        if(sthExist[player[i]->x][player[i]->y+1]==0)
                        {
                            sthExist[player[i]->x][player[i]->y+1]=5;
                        }
                        if(sthExist[player[i]->x][player[i]->y]==5)
                        {
                            sthExist[player[i]->x][player[i]->y]=0;
                        }
                        player[i]->y=player[i]->y+1;
                    }
                }
                break;
            case 3:
                if(player[i]->x < 14)
                {
                    if(sthExist[(player[i]->x)+1][player[i]->y]<=5)
                    {
                        if(sthExist[(player[i]->x)+1][player[i]->y]==0)
                        {
                            sthExist[(player[i]->x)+1][player[i]->y]=5;
                        }
                        if(sthExist[player[i]->x][player[i]->y]==5)
                        {
                            sthExist[player[i]->x][player[i]->y]=0;
                        }
                        player[i]->x=player[i]->x+1;
                    }
                }
                break;
            case 4:
                if(player[i]->y > 0)
                {
                    if(sthExist[player[i]->x][(player[i]->y)-1]<=5)
                    {
                        if(sthExist[player[i]->x][(player[i]->y)-1]==0)
                        {
                            sthExist[player[i]->x][(player[i]->y)-1]=5;
                        }
                        if(sthExist[player[i]->x][player[i]->y]==5)
                        {
                            sthExist[player[i]->x][player[i]->y]=0;
                        }
                        player[i]->y=player[i]->y-1;
                    }
                }
                break;
            default:
                if((sthExist[player[i]->x][player[i]->y]==0))
                {
                    sthExist[player[i]->x][player[i]->y]=5;
                }
                player[i]->direction=0;
                break;
        }
        player[i]->direction=0;
    }
}
void BombMan::bombAbout(void)
{
    Bomb *p=head->next;
    while(p)
    {
        p->lastTime=p->lastTime-1;
        int tmp=p->lastTime;
        if(tmp < (1000/FRAME))
        {
            if(tmp>(900/FRAME))
            {
                mdBomb->play();
            }
            sthExist[p->x][p->y]=2;
            if(tmp < (900/FRAME))
            {
                if(((p->x)>0)&&(sthExist[p->x-1][p->y]<7))
                    sthExist[(p->x)-1][p->y]=2;
                if(((p->x)<14)&&(sthExist[p->x+1][p->y]<7))
                    sthExist[(p->x)+1][p->y]=2;
                if(((p->y)>0)&&(sthExist[p->x][p->y-1]<7))
                    sthExist[p->x][(p->y)-1]=2;
                if(((p->y)<14)&&(sthExist[p->x][p->y+1]<7))
                    sthExist[p->x][(p->y)+1]=2;
                if(tmp < (800/FRAME))
                {
                    if(((p->x)>1)&&(sthExist[p->x-2][p->y]<7))
                        sthExist[(p->x)-2][p->y]=2;
                    if(((p->x)<13)&&(sthExist[p->x+2][p->y]<7))
                        sthExist[(p->x)+2][p->y]=2;
                    if(((p->y)>1)&&(sthExist[p->x][p->y-2]<7))
                        sthExist[p->x][(p->y)-2]=2;
                    if(((p->y)<13)&&(sthExist[p->x][p->y+2]<7))
                        sthExist[p->x][(p->y)+2]=2;
                    if(((player[p->master]->atackRange) > 2)&&(tmp < (700/FRAME)))
                    {
                        if(((p->x)>2)&&(sthExist[p->x-3][p->y]<7))
                            sthExist[(p->x)-3][p->y]=2;
                        if(((p->x)<12)&&(sthExist[p->x+3][p->y]<7))
                            sthExist[(p->x)+3][p->y]=2;
                        if(((p->y)>2)&&(sthExist[p->x][p->y-3]<7))
                            sthExist[p->x][(p->y)-3]=2;
                        if(((p->y)<12)&&(sthExist[p->x][p->y+3]<7))
                            sthExist[p->x][(p->y)+3]=2;
                    }
                    if(tmp == 0)
                    {
                        sthExist[p->x][p->y]=0;
                        if(((p->x)>0)&&(sthExist[p->x-1][p->y]<7))
                            sthExist[(p->x)-1][p->y]=0;
                        if(((p->x)<14)&&(sthExist[p->x+1][p->y]<7))
                            sthExist[(p->x)+1][p->y]=0;
                        if(((p->y)>0)&&(sthExist[p->x][p->y-1]<7))
                            sthExist[p->x][(p->y)-1]=0;
                        if(((p->y)<14)&&(sthExist[p->x][p->y+1]<7))
                            sthExist[p->x][(p->y)+1]=0;
                        if(((p->x)>1)&&(sthExist[p->x-2][p->y]<7))
                            sthExist[(p->x)-2][p->y]=0;
                        if(((p->x)<13)&&(sthExist[p->x+2][p->y]<7))
                            sthExist[(p->x)+2][p->y]=0;
                        if(((p->y)>1)&&(sthExist[p->x][p->y-2]<7))
                            sthExist[p->x][(p->y)-2]=0;
                        if(((p->y)<13)&&(sthExist[p->x][p->y+2]<7))
                            sthExist[p->x][(p->y)+2]=0;
                        if((player[p->master]->atackRange) > 2)
                        {
                            if(((p->x)>2)&&(sthExist[p->x-3][p->y]<7))
                                sthExist[(p->x)-3][p->y]=0;
                            if(((p->x)<12)&&(sthExist[p->x+3][p->y]<7))
                                sthExist[(p->x)+3][p->y]=0;
                            if(((p->y)>2)&&(sthExist[p->x][p->y-3]<7))
                                sthExist[p->x][(p->y)-3]=0;
                            if(((p->y)<12)&&(sthExist[p->x][p->y+3]<7))
                                sthExist[p->x][(p->y)+3]=0;
                        }
                        player[p->master]->bombCount=(player[p->master]->bombCount)-1;
                        Bomb *tp=head;
                        while((tp->next)!=p)
                            tp=tp->next;
                        tp->next=p->next;
                        free(p);
                    }
                }
            }
        }
        p=p->next;
    }
}
void BombMan::itemAbout()
{
    if(item->master >= 0)                   //激活态
    {
        item->lastTime=item->lastTime-1;
        if(item->lastTime == 0)
        {
            int x,y;
            free(item);
            do
            {
                x=qrand()%13+1;
                y=qrand()%13+1;
            }
            while(sthExist[x][y]!=0);
            item=new Item(qrand()%3,x,y);            //暂时只使用三个道具
            sthExist[x][y]=1;
        }
    }
    else
    {
        if(sthExist[item->x][item->y]==0)
        {
            free(item);
            head=new Bomb(0,0,2);
            item=new Item(-1,30000/FRAME);
        }
    }
}
void BombMan::enemyAbout()
{
    for(int i=0;i<ENEMYSIZE;i++)
    {
        if(sthExist[normalEnemy[i]->x][normalEnemy[i]->y]==2)
        {
            normalEnemy[i]->isAlive=0;
        }
        if(normalEnemy[i]->isAlive==1)
        {
            if((sthExist[normalEnemy[i]->x][normalEnemy[i]->y]==0)||
                    (sthExist[normalEnemy[i]->x][normalEnemy[i]->y]==5))
            {
                sthExist[normalEnemy[i]->x][normalEnemy[i]->y]=4;
            }
            enemyMove(normalEnemy[i]);
        }
    }
    for(int i=0;i<BOSSSIZE;i++)
    {
        if((sthExist[boss[i]->x][boss[i]->y]==0)||
                (sthExist[boss[i]->x][boss[i]->y]==5))
        {
            sthExist[boss[i]->x][boss[i]->y]=4;
        }
        enemyMove(boss[i]);
    }
}

void BombMan::judgement(void)
{
    for(int i=0;i<2;i++)
    {
        if(sthExist[player[i]->x][player[i]->y]==1)
        {
            switch(item->id)
            {
            case ID_COUNT:
                player[i]->bombCountLimit=player[i]->bombCountLimit+1;
                break;
            case ID_POWER:
                player[i]->atackRange=player[i]->atackRange+1;
                break;
            case ID_HEAL:
                player[i]->restLife=player[i]->restLife+5;
                lifeNum1->display(player[0]->restLife);
                lifeNum2->display(player[1]->restLife);
                break;
            default:
                i=i;
            }
            item->lastTime=30000/FRAME;
            item->master=i;
            sthExist[player[i]->x][player[i]->y]=5;
        }
        else if(sthExist[player[i]->x][player[i]->y]==2)
        {
            player[i]->restLife=(player[i]->restLife)-1;
            lifeNum1->display(player[0]->restLife);
            lifeNum2->display(player[1]->restLife);
            if((player[i]->restLife)<=0)
            {
                if(i==0)
                    gameOver(1);
                else
                    gameOver(0);
                return;
            }
        }
        else if(sthExist[player[i]->x][player[i]->y]==3)
        {
            player[i]->restLife=(player[i]->restLife)-1;
            lifeNum1->display(player[0]->restLife);
            lifeNum2->display(player[1]->restLife);
            if((player[i]->restLife)<=0)
            {
                if(i==0)
                    gameOver(1);
                else
                    gameOver(0);
                return;
            }
        }
        else if(sthExist[player[i]->x][player[i]->y]==4)
        {
            player[i]->restLife=(player[i]->restLife)-1;
            lifeNum1->display(player[0]->restLife);
            lifeNum2->display(player[1]->restLife);
            if((player[i]->restLife)<=0)
            {
                if(i==0)
                    gameOver(1);
                else
                    gameOver(0);
                return;
            }
        }

    }
    Bomb *p=head->next;
    while(p)
    {
        if(sthExist[p->x][p->y]==2)
        {
            if((p->lastTime)>1000/FRAME)
                p->lastTime=1000/FRAME;
        }
        p=p->next;
    }
}
void BombMan::enemyMove(Enemy *enemy)
{
    enemy->moveTime=enemy->moveTime-1;
    if(enemy->moveTime==0)
    {
        if(enemy->isBoss)
        {
            enemy->moveTime=400/FRAME;              //重装值
        }
        else
        {
            enemy->moveTime=500/FRAME;              //重装值
        }
        int x[2],y[2],z[2];
        for(int i=0;i<2;i++)                    //计算与两个玩家的距离
        {
            if((player[i]->x)>(enemy->x))
            {
                x[i]=(player[i]->x)-(enemy->x);
            }
            else
            {
                x[i]=(enemy->x)-(player[i]->x);
            }
            if((player[i]->y)>(enemy->y))
            {
                y[i]=(player[i]->y)-(enemy->y);
            }
            else
            {
                y[i]=(enemy->y)-(player[i]->y);
            }
            z[i]=x[i]*x[i]+y[i]*y[i];
        }
        if(z[0]<z[1])                                   //距离玩家1较近,则向玩家1移动
        {
            if(x[0]>y[0])                               //横坐标更长，横向靠近
            {
                if((player[0]->x)<(enemy->x))           //玩家1在enemy左边
                {
                    if(sthExist[enemy->x-1][enemy->y]<6)//如果enemy左边没石头，炸弹,则向左移动
                    {
                        if((sthExist[enemy->x-1][enemy->y]==0)||(sthExist[enemy->x-1][enemy->y]==5))
                        {
                            sthExist[enemy->x-1][enemy->y]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->x=enemy->x-1;
                    }
                    else                               //看看纵向移动
                    {
                        if((player[0]->y)<(enemy->y)) //如果玩家1在enemy上面
                        {
                            if(sthExist[enemy->x][enemy->y-1]<6)//如果enemy上边没石头，炸弹,则向上移动
                            {
                                if((sthExist[enemy->x][enemy->y-1]==0)||(sthExist[enemy->x][enemy->y-1]==5))
                                {
                                    sthExist[enemy->x][enemy->y-1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y-1;
                            }
                        }
                        else                           //如果玩家1在enemy下面
                        {
                            if(sthExist[enemy->x][enemy->y+1]<6)//如果enemy下边没石头，炸弹,则向下移动
                            {
                                if((sthExist[enemy->x][enemy->y+1]==0)||(sthExist[enemy->x][enemy->y+1]==5))
                                {
                                    sthExist[enemy->x][enemy->y+1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y+1;
                            }
                        }
                    }
                }
                else                                    //玩家1在enemy右边
                {
                    if(sthExist[enemy->x+1][enemy->y]<6)//如果enemy右边没石头，炸弹,则向右移动
                    {
                        if((sthExist[enemy->x+1][enemy->y]==0)||(sthExist[enemy->x+1][enemy->y]==5))
                        {
                            sthExist[enemy->x+1][enemy->y]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->x=enemy->x+1;
                    }
                    else                               //看看纵向移动
                    {
                        if((player[0]->y)<(enemy->y)) //如果玩家1在enemy上面
                        {
                            if(sthExist[enemy->x][enemy->y-1]<6)//如果enemy上边没石头，炸弹,则向上移动
                            {
                                if((sthExist[enemy->x][enemy->y-1]==0)||(sthExist[enemy->x][enemy->y-1]==5))
                                {
                                    sthExist[enemy->x][enemy->y-1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y-1;
                            }
                        }
                        else                           //如果玩家1在enemy下面
                        {
                            if(sthExist[enemy->x][enemy->y+1]<6)//如果enemy下边没石头，炸弹,则向下移动
                            {
                                if((sthExist[enemy->x][enemy->y+1]==0)||(sthExist[enemy->x][enemy->y+1]==5))
                                {
                                    sthExist[enemy->x][enemy->y+1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y+1;
                            }
                        }
                    }
                }
            }
            else                                        //纵坐标更长，纵向靠近
            {
                if((player[0]->y)<(enemy->y))           //玩家1在enemy上边
                {
                    if(sthExist[enemy->x][enemy->y-1]<6)//如果enemy上边没石头，炸弹,则向左移动
                    {
                        if((sthExist[enemy->x][enemy->y-1]==0)||(sthExist[enemy->x][enemy->y-1]==5))
                        {
                            sthExist[enemy->x][enemy->y-1]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->y=enemy->y-1;
                    }
                    else                               //看看横移动
                    {
                        if((player[0]->x)<(enemy->x)) //如果玩家1在enemy左面
                        {
                            if(sthExist[enemy->x-1][enemy->y]<6)//如果enemy左边没石头，炸弹,则向左移动
                            {
                                if((sthExist[enemy->x-1][enemy->y]==0)||(sthExist[enemy->x-1][enemy->y]==5))
                                {
                                    sthExist[enemy->x-1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x-1;
                            }
                        }
                        else                           //如果玩家1在enemy右面
                        {
                            if(sthExist[enemy->x+1][enemy->y]<6)//如果enemy右边没石头，炸弹,则向右移动
                            {
                                if((sthExist[enemy->x+1][enemy->y]==0)||(sthExist[enemy->x+1][enemy->y]==5))
                                {
                                    sthExist[enemy->x+1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x+1;
                            }
                        }
                    }
                }
                else                                    //玩家1在enemy下边
                {
                    if(sthExist[enemy->x][enemy->y+1]<6)//如果enemy下边没石头，炸弹,则向下移动
                    {
                        if((sthExist[enemy->x][enemy->y+1]==0)||(sthExist[enemy->x][enemy->y+1]==5))
                        {
                            sthExist[enemy->x][enemy->y+1]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->y=enemy->y+1;
                    }
                    else                               //看看横向移动
                    {
                        if((player[0]->x)<(enemy->x)) //如果玩家1在enemy左面
                        {
                            if(sthExist[enemy->x-1][enemy->y]<6)//如果enemy左边没石头，炸弹,则向左移动
                            {
                                if((sthExist[enemy->x-1][enemy->y]==0)||(sthExist[enemy->x-1][enemy->y]==5))
                                {
                                    sthExist[enemy->x-1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x-1;
                            }
                        }
                        else                           //如果玩家1在enemy右面
                        {
                            if(sthExist[enemy->x+1][enemy->y]<6)//如果enemy右边没石头，炸弹,则向右移动
                            {
                                if((sthExist[enemy->x+1][enemy->y]==0)||(sthExist[enemy->x+1][enemy->y]==5))
                                {
                                    sthExist[enemy->x+1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x+1;
                            }
                        }
                    }
                }
            }
        }
        else                                            //距离玩家2较近，则向玩家2移动
        {
            if(x[1]>y[1])                               //横坐标更长，横向靠近
            {
                if((player[1]->x)<(enemy->x))           //玩家2在enemy左边
                {
                    if(sthExist[enemy->x-1][enemy->y]<6)//如果enemy左边没石头，炸弹,则向左移动
                    {
                        if((sthExist[enemy->x-1][enemy->y]==0)||(sthExist[enemy->x-1][enemy->y]==5))
                        {
                            sthExist[enemy->x-1][enemy->y]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->x=enemy->x-1;
                    }
                    else                               //看看纵向移动
                    {
                        if((player[1]->y)<(enemy->y)) //如果玩家2在enemy上面
                        {
                            if(sthExist[enemy->x][enemy->y-1]<6)//如果enemy上边没石头，炸弹,则向上移动
                            {
                                if((sthExist[enemy->x][enemy->y-1]==0)||(sthExist[enemy->x][enemy->y-1]==5))
                                {
                                    sthExist[enemy->x][enemy->y-1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y-1;
                            }
                        }
                        else                           //如果玩家2在enemy下面
                        {
                            if(sthExist[enemy->x][enemy->y+1]<6)//如果enemy下边没石头，炸弹,则向下移动
                            {
                                if((sthExist[enemy->x][enemy->y+1]==0)||(sthExist[enemy->x][enemy->y+1]==5))
                                {
                                    sthExist[enemy->x][enemy->y+1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y+1;
                            }
                        }
                    }
                }
                else                                    //玩家2在enemy右边
                {
                    if(sthExist[enemy->x+1][enemy->y]<6)//如果enemy右边没石头，炸弹,则向右移动
                    {
                        if((sthExist[enemy->x+1][enemy->y]==0)||(sthExist[enemy->x+1][enemy->y]==5))
                        {
                            sthExist[enemy->x+1][enemy->y]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->x=enemy->x+1;
                    }
                    else                               //看看纵向移动
                    {
                        if((player[1]->y)<(enemy->y)) //如果玩家2在enemy上面
                        {
                            if(sthExist[enemy->x][enemy->y-1]<6)//如果enemy上边没石头，炸弹,则向上移动
                            {
                                if((sthExist[enemy->x][enemy->y-1]==0)||(sthExist[enemy->x][enemy->y-1]==5))
                                {
                                    sthExist[enemy->x][enemy->y-1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y-1;
                            }
                        }
                        else                           //如果玩家2在enemy下面
                        {
                            if(sthExist[enemy->x][enemy->y+1]<6)//如果enemy下边没石头，炸弹,则向下移动
                            {
                                if((sthExist[enemy->x][enemy->y+1]==0)||(sthExist[enemy->x][enemy->y+1]==5))
                                {
                                    sthExist[enemy->x][enemy->y+1]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->y=enemy->y+1;
                            }
                        }
                    }
                }
            }
            else                                        //纵坐标更长，纵向靠近
            {
                if((player[1]->y)<(enemy->y))           //玩家2在enemy上边
                {
                    if(sthExist[enemy->x][enemy->y-1]<6)//如果enemy上边没石头，炸弹,则向左移动
                    {
                        if((sthExist[enemy->x][enemy->y-1]==0)||(sthExist[enemy->x][enemy->y-1]==5))
                        {
                            sthExist[enemy->x][enemy->y-1]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->y=enemy->y-1;
                    }
                    else                               //看看横移动
                    {
                        if((player[1]->x)<(enemy->x)) //如果玩家2在enemy左面
                        {
                            if(sthExist[enemy->x-1][enemy->y]<6)//如果enemy左边没石头，炸弹,则向左移动
                            {
                                if((sthExist[enemy->x-1][enemy->y]==0)||(sthExist[enemy->x-1][enemy->y]==5))
                                {
                                    sthExist[enemy->x-1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x-1;
                            }
                        }
                        else                           //如果玩家2在enemy右面
                        {
                            if(sthExist[enemy->x+1][enemy->y]<6)//如果enemy右边没石头，炸弹,则向右移动
                            {
                                if((sthExist[enemy->x+1][enemy->y]==0)||(sthExist[enemy->x+1][enemy->y]==5))
                                {
                                    sthExist[enemy->x+1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x+1;
                            }
                        }
                    }
                }
                else                                    //玩家2在enemy下边
                {
                    if(sthExist[enemy->x][enemy->y+1]<6)//如果enemy下边没石头，炸弹,则向下移动
                    {
                        if((sthExist[enemy->x][enemy->y+1]==0)||(sthExist[enemy->x][enemy->y+1]==5))
                        {
                            sthExist[enemy->x][enemy->y+1]=4;
                        }
                        if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                        {
                            sthExist[enemy->x][enemy->y]=0;
                        }
                        enemy->y=enemy->y+1;
                    }
                    else                               //看看横向移动
                    {
                        if((player[1]->x)<(enemy->x)) //如果玩家2在enemy左面
                        {
                            if(sthExist[enemy->x-1][enemy->y]<6)//如果enemy左边没石头，炸弹,则向左移动
                            {
                                if((sthExist[enemy->x-1][enemy->y]==0)||(sthExist[enemy->x-1][enemy->y]==5))
                                {
                                    sthExist[enemy->x-1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x-1;
                            }
                        }
                        else                           //如果玩家2在enemy右面
                        {
                            if(sthExist[enemy->x+1][enemy->y]<6)//如果enemy右边没石头，炸弹,则向右移动
                            {
                                if((sthExist[enemy->x+1][enemy->y]==0)||(sthExist[enemy->x+1][enemy->y]==5))
                                {
                                    sthExist[enemy->x+1][enemy->y]=4;
                                }
                                if((sthExist[enemy->x][enemy->y]==4)||(sthExist[enemy->x][enemy->y]==5))
                                {
                                    sthExist[enemy->x][enemy->y]=0;
                                }
                                enemy->x=enemy->x+1;
                            }
                        }
                    }
                }
            }
        }
    }
}
