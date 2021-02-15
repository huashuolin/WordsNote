#include "MainWidget.h"
#include <QApplication>
#include <QSharedMemory>
#include "log.h"
#include "LoginWnd.h"



int ShowMainWnd(const QString& qstrUserName)
{
    CMainWidget* pW = new CMainWidget();
    pW->show();
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    //注册MessageHandler，进行日志打印
    qInstallMessageHandler(outputMessage);

    //限制程序只启动一次
//    QSharedMemory singleton(a.applicationName());
//    if(!singleton.create(1))
//    {
//        return 0;
//    }


    CLoginWnd loginWnd;
    if(QDialog::Accepted == loginWnd.exec())
    {
        CMainWidget* pW = new CMainWidget();
        pW->show();
    }
    else
    {
        return 0;
    }

    return a.exec();
}
