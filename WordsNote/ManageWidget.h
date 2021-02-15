/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       ManageWidget.h
*   Description:    主窗口左侧的管理窗口
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef MANAGEWIDGET_H
#define MANAGEWIDGET_H

#include <QWidget>
#include "DiaryManagerWidget.h"
#include "DocManagerWidget.h"

namespace Ui {
class CManageWidget;
}

class CThreadShowUserImage;
class CManageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CManageWidget(QWidget *parent = 0);
    virtual ~CManageWidget();

    /**@fn         ShowUserImage
    *  @brief      显示用户头像
    *  @return     void
    */
    void ShowUserImage();

private:
    Ui::CManageWidget *ui;

    CDiaryManagerWidget* m_pDiaryManagerWnd;//日记管理界面
    CDocManagerWidget* m_pDocManagerWnd;//文档管理界面

public slots:
    /**@fn         Slot_ClickDiaryBtn
    *  @brief      槽函数：点击切换到日记的按钮
    *  @para       bool bCheck
    *  @return     void
    */
    void Slot_ClickDiaryBtn(bool bCheck);

    /**@fn         Slot_ClickDocBtn
    *  @brief      槽函数：点击切换到文档的按钮
    *  @para       bool bCheck
    *  @return     void
    */
    void Slot_ClickDocBtn(bool bCheck);

signals:
    /**@fn         Signal_OpenDiaryFile
    *  @brief      信号： 打开日记文件
    *  @para       const QVariant& variant，由RecordInfo转换，RecordInfo为记录信息
    *  @return     void
    */
    void Signal_OpenDiaryFile(const QVariant& variant);

    /**@fn         Signal_OpenDocFile
    *  @brief      信号： 打开文档文件
    *  @para       const QVariant& variant，由RecordInfo转换，RecordInfo为记录信息
    *  @return     void
    */
    void Signal_OpenDocFile(const QVariant&);

    /**@fn         Signal_SetEditDisableAndBlank
    *  @brief      信号： 设置文本编辑为空且不可用
    *  @return     void
    */
    void Signal_SetEditDisableAndBlank();

    /**@fn         Signal_TreeNodeNameChanged
    *  @brief      信号： 树节点名称发生改变
    *  @para       const QString& qstrNewTreeNodeName 新的树节点名称
    *  @return     void
    */
    void Signal_TreeNodeNameChanged(const QString& qstrNewTreeNodeName);

    /**@fn         Signal_UpdateDiaryNodeInfo
    *  @brief      信息：更新日记信息
    *  @param      QVariant var 日记信息
    *  @return     void
    */
    void Signal_UpdateDiaryNodeInfo(QVariant var);

    /**@fn         Signal_UpdateDocumentNodeInfo
    *  @brief      信息：更新文档信息
    *  @param      QVariant var 文档信息
    *  @return     void
    */
    void Signal_UpdateDocumentNodeInfo(QVariant var);
};

#endif // MANAGEWIDGET_H
