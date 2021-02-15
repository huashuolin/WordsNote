/**************************************************************************
*   Copyright huashuolin001
*   FileName:       MainWidget.h
*   Description:    主界面
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0          09/07/2020        huashuolin001          创建
**************************************************************************/


#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "ManageWidget.h"
#include "TextOperateWidget.h"

namespace Ui {
class CMainWidget;
}

class CMainWidget : public QWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~CMainWidget();

private:
    Ui::CMainWidget *ui;

    CManageWidget* m_pManagerWnd; //管理界面
    CTextOperateWidget* m_pTextOperateWidget; //文本界面
};

#endif // MAINWIDGET_H
