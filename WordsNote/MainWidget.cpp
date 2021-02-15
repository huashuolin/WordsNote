#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QtGlobal>

#ifdef Q_OS_MAC
int g_iData = 100;
#endif

#ifdef Q_OS_LINUX
int g_iData = 101;
#endif

#ifdef Q_OS_WIN32
int g_iData = 102;
#endif



CMainWidget::CMainWidget(QWidget *parent,  Qt::WindowFlags flags)
    : QWidget(parent, flags),
    ui(new Ui::CMainWidget),
    m_pManagerWnd(nullptr),
    m_pTextOperateWidget(nullptr)
{
    ui->setupUi(this);

//#ifdef Q_OS_WIN32
//    setWindowFlags(Qt::FramelessWindowHint);
//#endif

    int iTest = g_iData;

    {
        m_pManagerWnd = new CManageWidget(ui->widget_left);
        //加上布局
        QVBoxLayout* pVBoxLayout = new QVBoxLayout(ui->widget_left);
        pVBoxLayout->setMargin(0);
        pVBoxLayout->addWidget(m_pManagerWnd);
    }


    {
        m_pTextOperateWidget = new CTextOperateWidget(ui->widget_right);
        QVBoxLayout* pVBoxLayout = new QVBoxLayout(ui->widget_right);
        pVBoxLayout->setMargin(0);
        pVBoxLayout->addWidget(m_pTextOperateWidget);
    }

    connect(m_pManagerWnd, SIGNAL(Signal_OpenDiaryFile(const QVariant&)), m_pTextOperateWidget, SLOT(Slot_OpenDiaryFile(const QVariant&)));
    connect(m_pManagerWnd, SIGNAL(Signal_OpenDocFile(const QVariant&)), m_pTextOperateWidget, SLOT(Slot_OpenDocFile(const QVariant&)));
    //connect(m_pManagerWnd, SIGNAL(Signal_DocNameChanged(QString)), m_pTextOperateWidget, SLOT(Slot_DocNameChanged(QString)));
    connect(m_pManagerWnd, SIGNAL(Signal_SetEditDisableAndBlank()), m_pTextOperateWidget, SLOT(Slot_EditDisableAndBlank()));
    connect(m_pManagerWnd, SIGNAL(Signal_TreeNodeNameChanged(QString)), m_pTextOperateWidget, SLOT(Slot_TreeNodeNameChanged(QString)));
    connect(m_pTextOperateWidget, SIGNAL(Signal_UpdateDiaryNodeInfo(QVariant)), m_pManagerWnd, SIGNAL(Signal_UpdateDiaryNodeInfo(QVariant)));
    connect(m_pTextOperateWidget, SIGNAL(Signal_UpdateDocumentNodeInfo(QVariant)), m_pManagerWnd, SIGNAL(Signal_UpdateDocumentNodeInfo(QVariant)));
}

CMainWidget::~CMainWidget()
{
    delete ui;
}
