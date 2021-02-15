/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       LoginAdditionWidget.h
*   Description:    登陆窗口的附加框（用于扩展展示记住用户名、注册等功能按钮的附件界面）
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef LOGINADDITIONWIDGET_H
#define LOGINADDITIONWIDGET_H

#include "RegisterWnd.h"

#include <QWidget>

namespace Ui {
class CLoginAdditionWidget;
}

class CLoginAdditionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CLoginAdditionWidget(QWidget *parent = 0);
    virtual ~CLoginAdditionWidget();

private:
    Ui::CLoginAdditionWidget *ui;

    CRegisterWnd* m_pRegisterWnd; //注册窗口

public slots:
    /**@fn         Slot_OpenRegisterWnd
    *  @brief      信号： 打开注册窗口
    *  @return     void
    */
    void Slot_OpenRegisterWnd();
};

#endif // LOGINADDITIONWIDGET_H
