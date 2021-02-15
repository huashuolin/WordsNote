#include "ManageWidget.h"
#include "ui_ManageWidget.h"
#include "GlobalData.h"
#include "DataManager.h"

CManageWidget::CManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CManageWidget),
    m_pDiaryManagerWnd(nullptr),
    m_pDocManagerWnd(nullptr)
{
    ui->setupUi(this);

    m_pDiaryManagerWnd = new CDiaryManagerWidget(ui->stackedWidget);
    m_pDocManagerWnd = new CDocManagerWidget(ui->stackedWidget);

    //清空QStackWidget自带的page
    int iDefaultCount = ui->stackedWidget->count();
    for(int i = iDefaultCount - 1; i >= 0; i--)
    {
        QWidget* pWidget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(pWidget);
    }

    int iTest1 = ui->stackedWidget->count();;

    ui->stackedWidget->addWidget(m_pDiaryManagerWnd);
    ui->stackedWidget->addWidget(m_pDocManagerWnd);

    //ShowUserImage();

    //绑定信号槽
    connect(ui->toolButton_diary, SIGNAL(clicked(bool)), this, SLOT(Slot_ClickDiaryBtn(bool)));
    connect(ui->toolButton_document, SIGNAL(clicked(bool)), this, SLOT(Slot_ClickDocBtn(bool)));

    connect(m_pDiaryManagerWnd, SIGNAL(Signal_OpenDiaryFile(QVariant)), this, SIGNAL(Signal_OpenDiaryFile(QVariant)));
    connect(m_pDocManagerWnd, SIGNAL(Signal_OpenDocFile(QVariant)), this, SIGNAL(Signal_OpenDocFile(QVariant)));
    connect(m_pDocManagerWnd, SIGNAL(Signal_TreeNodeNameChanged(QString)), this, SIGNAL(Signal_TreeNodeNameChanged(QString)));
    connect(m_pDiaryManagerWnd, SIGNAL(Signal_SetEditDisableAndBlank()), this, SIGNAL(Signal_SetEditDisableAndBlank()));
    connect(m_pDocManagerWnd, SIGNAL(Signal_SetEditDisableAndBlank()), this, SIGNAL(Signal_SetEditDisableAndBlank()));
    connect(m_pDiaryManagerWnd, SIGNAL(Signal_TreeNodeNameChanged(QString)), this, SIGNAL(Signal_TreeNodeNameChanged(QString)));
    connect(this, SIGNAL(Signal_UpdateDiaryNodeInfo(QVariant)), m_pDiaryManagerWnd, SLOT(Slot_UpateDiaryNodeInfo(QVariant)));
    connect(this, SIGNAL(Signal_UpdateDocumentNodeInfo(QVariant)), m_pDocManagerWnd, SLOT(Slot_UpdateDocNodeInfo(QVariant)));

    ShowUserImage();
}

CManageWidget::~CManageWidget()
{
    delete ui;
}

void CManageWidget::ShowUserImage()
{
    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    if(stuUserInfo.qstrImagePath.length() > 0)
    {
        QImage img;
        if(true == img.load(stuUserInfo.qstrImagePath)) //加载图像
        {
            //QImage imgScaled ;
            //imgScaled = img.scaled(ui->label_userImage->width(), ui->label_userImage->height(), Qt::KeepAspectRatio);
            //qDebug("imgScaled width : %d, height : %d", ui->label_userImage->width(), ui->label_userImage->height());
            ui->label_userImage->setScaledContents(true);
            ui->label_userImage->setPixmap(QPixmap::fromImage(img));
            ui->label_userImage->repaint();
        }
    }
}

void CManageWidget::Slot_ClickDiaryBtn(bool bCheck)
{
    ui->stackedWidget->setCurrentIndex(0);
}
void CManageWidget::Slot_ClickDocBtn(bool bCheck)
{
    ui->stackedWidget->setCurrentIndex(1);
}
