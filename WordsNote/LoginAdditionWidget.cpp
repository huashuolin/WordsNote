#include "LoginAdditionWidget.h"
#include "ui_LoginAdditionWidget.h"

CLoginAdditionWidget::CLoginAdditionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLoginAdditionWidget),
    m_pRegisterWnd(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    connect(ui->pushButton_register, SIGNAL(clicked(bool)), this, SLOT(Slot_OpenRegisterWnd()));
}

CLoginAdditionWidget::~CLoginAdditionWidget()
{
    delete ui;
}

void CLoginAdditionWidget::Slot_OpenRegisterWnd()
{
    if(nullptr == m_pRegisterWnd)
    {
        m_pRegisterWnd = new(std::nothrow) CRegisterWnd(this);
    }
    m_pRegisterWnd->show();
}
