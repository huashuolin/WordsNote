#include "RegisterWnd.h"
#include "ui_RegisterWnd.h"
#include "DataManager.h"

#include <QFileDialog>
#include <QMessageBox>

CRegisterWnd::CRegisterWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRegisterWnd)
{
    ui->setupUi(this);
#ifdef Q_OS_WIN32
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
#endif

#ifdef Q_OS_MAC
    setWindowFlags(Qt::Dialog);
#endif

    connect(ui->pushButton_register, SIGNAL(clicked(bool)), this, SLOT(Slot_RegisterBtnClicked()));
    connect(ui->pushButton_cancle, SIGNAL(clicked(bool)), this, SLOT(Slot_CancleBtnClicked()));

    ui->label_image->installEventFilter(this);
}

CRegisterWnd::~CRegisterWnd()
{
    delete ui;
}

void CRegisterWnd::Slot_RegisterBtnClicked()
{
    do
    {
        //1. 验证输入数据是否有效
        QString qstrUserName = ui->lineEdit_userName->text();
        QString qstrPwd = ui->lineEdit_userPwd->text();
        QString qstrPwdConfirm = ui->lineEdit_userPwdConfirm->text();

        if(qstrUserName.isEmpty())
        {
            QMessageBox messageBox;
            messageBox.setText("用户名不能为空");
            messageBox.exec();
            break;
        }

        if(qstrPwd.isEmpty())
        {
            QMessageBox messageBox;
            messageBox.setText("密码不能为空");
            messageBox.exec();
            break;
        }

        if(qstrPwdConfirm.isEmpty())
        {
            QMessageBox messageBox;
            messageBox.setText("确认密码不能为空");
            messageBox.exec();
            break;
        }

        if(true == CDataManager::GetInstance()->UserDB_IsUserExist(qstrUserName))
        {
            QMessageBox messageBox;
            messageBox.setText("该用户已存在");
            messageBox.exec();
            break;
        }

        if(qstrPwd != qstrPwdConfirm)
        {
            QMessageBox messageBox;
            messageBox.setText("两次密码输入不一样");
            messageBox.exec();
            break;
        }

        //2. 如果有头像，则拷贝头像到程序目录
        QString qstrUserImageFullPath;
        if(false == m_qstrImagePath.isEmpty())
        {
            int iPosSeparator = m_qstrImagePath.lastIndexOf(".");
            QString qstrImageFormat = m_qstrImagePath.mid(iPosSeparator, m_qstrImagePath.length() - iPosSeparator);

            QString qstrCurrentPath = QDir::currentPath();
            qstrUserImageFullPath =  qstrCurrentPath + "/UserImage/" + qstrUserName + qstrImageFormat;
            QString qstrDir = qstrCurrentPath + "/UserImage";

            QDir dir;
            if (!dir.exists(qstrDir))
            {
                bool res = dir.mkpath(qstrDir);
            }
            bool bCopy = QFile::copy(m_qstrImagePath, qstrUserImageFullPath);
        }

        //3. 有效数据，则添加用户
        bool bAddRst = CDataManager::GetInstance()->UserDB_AddUser(qstrUserName, qstrPwd, qstrUserImageFullPath);
        if(true == bAddRst)
        {
            QMessageBox messageBox;
            messageBox.setText("注册用户成功");
            messageBox.exec();
            close();
        }

    }while(0);



}

void CRegisterWnd::Slot_CancleBtnClicked()
{
    close();
}


bool CRegisterWnd::eventFilter(QObject *watched, QEvent *event)
{
    do
    {
        if (event->type() == QEvent::MouseButtonRelease && ui->label_image == (QLabel*)watched)
        {
            //m_osdtext1 == (QLabel*)watched;
            QFileDialog* pFileDialog = new QFileDialog(this);
            pFileDialog->setWindowTitle(QStringLiteral("选择图片文件"));
            pFileDialog->setDirectory(".");//设置默认文件路径
            pFileDialog->setNameFilter(tr("File(*.png *.jpg)"));//设置文件过滤器
            //pFileDialog->setFileMode(QFileDialog::ExistingFiles);//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
            pFileDialog->setViewMode(QFileDialog::Detail); //设置视图模式
            //打印所有选择的文件的路径
            QStringList fileFullNames;
            if (pFileDialog->exec())
            {
                fileFullNames = pFileDialog->selectedFiles();
                if(fileFullNames.size() > 0)
                {
                    m_qstrImagePath = fileFullNames.at(0);
                    QImage img;
                    if(false == img.load(m_qstrImagePath)) //加载图像
                    {
                        break;
                    }
                    //QImage imgScaled ;
                    //imgScaled = img.scaled(ui->label_image->width(), ui->label_image->height(), Qt::KeepAspectRatio);


                    ui->label_image->setPixmap(QPixmap::fromImage(img));
                    ui->label_image->repaint();
                }

            }
            else
            {

            }
        }
    }while(0);
    return QWidget::eventFilter(watched, event);
}
