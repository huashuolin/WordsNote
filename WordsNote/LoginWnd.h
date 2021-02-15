/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       LoginWnd.h
*   Description:    用户登陆窗口
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef LOGINWND_H
#define LOGINWND_H
#pragma execution_character_set("utf-8")

#include"RegisterWnd.h"

#include <QDialog>

namespace Ui {
class CLoginWnd;
}

class CLoginWnd : public QDialog
{
    Q_OBJECT

public:
    explicit CLoginWnd(QWidget *parent = 0);
    virtual ~CLoginWnd();

private:
    Ui::CLoginWnd *ui;
    CRegisterWnd* m_pRegisterWnd; //注册窗口

    /**@fn         ShowAdditionWnd
    *  @brief      是否展示附加窗口
    *  @para       bool bStatus true代表展示附加窗口，false代表隐藏附加窗口
    *  @return     void
    */
    void ShowAdditionWnd(bool bStatus);

public slots:
    /**@fn         Slot_LoginBtnClicked
    *  @brief      槽函数： 登陆按钮点击
    *  @return     void
    */
    void Slot_LoginBtnClicked();

    /**@fn         Slot_ShowAdditionalWidgetBtnClicked
    *  @brief      槽函数： 向下扩展显示附加框的按钮点击
    *  @return     void
    */
    void Slot_ShowAdditionalWidgetBtnClicked();

    /**@fn         Slot_UserNameChanged
    *  @brief      槽函数： 输入的用户名发生改变
    *  @para       onst QString &text 改变后的用户名
    *  @return     void
    */
    void Slot_UserNameChanged(const QString &text);


    /**@fn         Slot_OpenRegisterWnd
    *  @brief      槽函数： 打开注册窗口
    *  @return     void
    */
    void Slot_OpenRegisterWnd();

    /**@fn         Slot_CloseWnd
    *  @brief      槽函数： 关闭窗口
    *  @return     void
    */
    void Slot_CloseWnd();

    /**@fn         Slot_MinimizeWnd
    *  @brief      槽函数： 最小化窗口
    *  @return     void
    */
    void Slot_MinimizeWnd();

};

#endif // LOGINWND_H
