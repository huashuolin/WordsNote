/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       RegisterWnd.h
*   Description:    用户注册窗口
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef REGISTERWND_H
#define REGISTERWND_H
#pragma execution_character_set("utf-8")

#include <QWidget>

namespace Ui {
class CRegisterWnd;
}

class CRegisterWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CRegisterWnd(QWidget *parent = 0);
    virtual ~CRegisterWnd();

private:
    Ui::CRegisterWnd *ui;
    QString m_qstrImagePath;//上传头像的全路径

public slots:
    /**@fn         Slot_RegisterBtnClicked
    *  @brief      槽函数： 注册按钮点击
    *  @return     void
    */
    void Slot_RegisterBtnClicked();

    /**@fn         Slot_CancleBtnClicked
    *  @brief      槽函数：取消按钮点击
    *  @return     void
    */
    void Slot_CancleBtnClicked();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
};

#endif // REGISTERWND_H
