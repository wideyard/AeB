#include<QApplication>
#include<QMessageBox>
#include<QSplashScreen>
#include<QPixmap>
#include<QDesktopWidget>
#include"mianwork.h"
#include"login.h"
int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    login splash;
    splash.show();
    return app.exec();
}
