#include "LoginWnd.h"
#include "ui_LoginWnd.h"
#include "DataManager.h"
#include "MainWidget.h"
#include "GlobalData.h"
#include <QPaintEvent>
#include <QMessageBox>

CLoginWnd::CLoginWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginWnd),
    m_pRegisterWnd(nullptr)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN32
    setWindowFlags(Qt::FramelessWindowHint);
#endif

#ifdef Q_OS_MAC
    ui->widget_title->setVisible(false);
    setFixedHeight(352);
#endif

    connect(ui->toolButton_showAdditionalWidget, SIGNAL(clicked(bool)), this, SLOT(Slot_ShowAdditionalWidgetBtnClicked()));
    connect(ui->lineEdit_userName, SIGNAL(textChanged(const QString&)), this, SLOT(Slot_UserNameChanged(QString)));
    connect(ui->toolButton_login, SIGNAL(clicked(bool)), this, SLOT(Slot_LoginBtnClicked()));
    connect(ui->pushButton_min, SIGNAL(clicked(bool)), this, SLOT(Slot_MinimizeWnd()));
    connect(ui->pushButton_close, SIGNAL(clicked(bool)), this, SLOT(Slot_CloseWnd()));
    connect(ui->pushButton_register, SIGNAL(clicked(bool)), this, SLOT(Slot_OpenRegisterWnd()));

    CDataManager::GetInstance()->Init();

    //初始化默认头像
    {
        QImage img;
        if(true == img.load(":/images/login/default_iamge.png")) //加载图像
        {
            //QImage imgScaled ;
            //imgScaled = img.scaled(ui->label_image->width(), ui->label_image->height(), Qt::KeepAspectRatio);
            ui->label_image->setPixmap(QPixmap::fromImage(img));
            ui->label_image->repaint();
        }
    }

    ShowAdditionWnd(false);
}

CLoginWnd::~CLoginWnd()
{
    CDataManager::GetInstance()->UnInit();
    delete ui;
}

void CLoginWnd::ShowAdditionWnd(bool bStatus)
{
#ifdef Q_OS_WIN32
    if(true == bStatus)
    {
        ui->widget_addition->setVisible(true);
        setFixedHeight(384);
    }
    else
    {
        ui->widget_addition->setVisible(false);
        setFixedHeight(344);
    }
#endif

#ifdef Q_OS_MAC
    if(true == bStatus)
    {
        ui->widget_addition->setVisible(true);
        setFixedHeight(352);
    }
    else
    {
        ui->widget_addition->setVisible(false);
        setFixedHeight(312);
    }
#endif
}

//当前要解决三个问题
//1. 界面下拉效果
//2. 登陆界面到主界面的跳转
//3. 用户名、密码的验证
//4. 用户注册
//5. 数据库的对接部分
void CLoginWnd::Slot_LoginBtnClicked()
{
    do
    {
        QString qstrUserName = ui->lineEdit_userName->text();
        QString qstrPwd = ui->lineEdit_userPwd->text();
        if(qstrUserName.isEmpty() || qstrPwd.isEmpty())
        {
            QMessageBox messagebox;
            messagebox.setText("用户名或者密码不能为空");
            messagebox.exec();
            break;
        }

        bool bUserExist = CDataManager::GetInstance()->UserDB_IsUserExist(qstrUserName);
        if(false == bUserExist)
        {
            QMessageBox messagebox;
            messagebox.setText("用户名不存在");
            messagebox.exec();
            break;
        }

        bool bRst = CDataManager::GetInstance()->UserDB_VertifyUser(qstrUserName, qstrPwd);
        if(false == bRst)
        {
            QMessageBox messagebox;
            messagebox.setText("密码错误");
            messagebox.exec();
            break;

        }
        else
        {
            CGlobalData::GetInstance()->Fun_SetUserName(qstrUserName);
            accept();
        }
    }while (0);

}

void CLoginWnd::Slot_ShowAdditionalWidgetBtnClicked()
{
    if(true == ui->widget_addition->isVisible())
    {
        ShowAdditionWnd(false);
    }
    else
    {
        ShowAdditionWnd(true);
    }



//    if(nullptr == m_pAdditionalWidget)
//    {
//        m_pAdditionalWidget = new(std::nothrow) CLoginAdditionWidget(this);
//    }

//    if(m_pAdditionalWidget->isVisible())
//    {
//        m_pAdditionalWidget->hide();
//        ui->toolButton_showAdditionalWidget->setIcon(QIcon(":/images/login/arrow_down.png"));
//    }
//    else
//    {
//        m_pAdditionalWidget->show();
//        ui->toolButton_showAdditionalWidget->setIcon(QIcon(":/images/login/arrow_up.png"));
//    }
}

void CLoginWnd::Slot_UserNameChanged(const QString &text)
{
    if(true == CDataManager::GetInstance()->UserDB_IsUserExist(text))
    {
        UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(text);
        QImage img;
        if(true == img.load(stuUserInfo.qstrImagePath)) //加载图像
        {
            //QImage imgScaled ;
            //imgScaled = img.scaled(ui->label_image->width(), ui->label_image->height(), Qt::KeepAspectRatio);
            ui->label_image->setPixmap(QPixmap::fromImage(img));
            ui->label_image->repaint();
        }
    }


}

void CLoginWnd::Slot_OpenRegisterWnd()
{
    if(nullptr == m_pRegisterWnd)
    {
        m_pRegisterWnd = new(std::nothrow) CRegisterWnd(this);
    }
    m_pRegisterWnd->show();
}

void CLoginWnd::Slot_CloseWnd()
{
    close();
}

void CLoginWnd::Slot_MinimizeWnd()
{
    showMinimized();
}
