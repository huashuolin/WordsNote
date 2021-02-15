/**************************************************************************
*   Copyright huashuolin001
*   FileName:       DocManagerWidget.h
*   Description:    文档管理目录
*   Author:         huashuolin001
*   Date:           09/07/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0          09/07/2020        huashuolin001          创建
**************************************************************************/

#ifndef DOCMANAGERWIDGET_H
#define DOCMANAGERWIDGET_H
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>

namespace Ui {
class CDocManagerWidget;
}

class CDocManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDocManagerWidget(QWidget *parent = 0);
    virtual ~CDocManagerWidget();

private:
    Ui::CDocManagerWidget *ui;
    QStandardItemModel* m_pTreeModelDoc; //文档目录树模型

    QMenu m_menu; //日记菜单
    QAction* m_actionAdd; //添加文档
    QAction* m_actionDel; //删除文档
    QAction* m_actionReName; //重命名文档

    /**@fn         InitTreeData
    *  @brief      初始化树（读取数据库中所有文档构建树结构）
    *  @return     void
    */
    void InitTreeData();

    /**@fn         AddTreeItem_Sub
    *  @brief      在文档树中添加子节点
    *  @para       StandardItem* pParentItem 父节点
    *  @para       int iId 文档ID
    *  @para       int iParentId 父节点ID
    *  @para       const QString& qstrNodeName 树节点名称
    *  @para       const QString& qstrFilePath 文件存储路径
    *  @return     void
    */
    void AddTreeItem_Sub(QStandardItem* pParentItem, int iId, int iParentId, const QString& qstrNodeName, const QString& qstrFilePath);

    /**@fn         OpenDoc
    *  @brief      打开文档
    *  @para       const QStandardItem* pItem 文档树节点
    *  @return     void
    */
    void OpenDoc(const QStandardItem* pItem);
public slots:

    /**@fn         Slot_NewTreeNode
    *  @brief      槽函数：新增树节点
    *  @return     void
    */
    void Slot_NewTreeNode();

    /**@fn         Slot_TreeViewClick
    *  @brief      槽函数：点击树节点
    *  @para       const QModelIndex& modelIndex 树节点索引
    *  @return     void
    */
    void Slot_TreeViewClick(const QModelIndex& modelIndex);

    /**@fn         Slot_ItemChanged
    *  @brief      槽函数：树节点改变
    *  @para       QStandardItem* pItem 改变的节点
    *  @return     void
    */
    void Slot_ItemChanged(QStandardItem* pItem);

    /**@fn         Slot_ShowContextMenu
    *  @brief      槽函数：展示右键菜单
    *  @para       const QPoint& pos 鼠标位置
    *  @return     void
    */
    void Slot_ShowContextMenu(const QPoint& pos);

    /**@fn         Slot_ActionAddNote
    *  @brief      槽函数：添加文档
    *  @return     void
    */
    void Slot_ActionAddNote();

    /**@fn         Slot_ActionDelNote
    *  @brief      槽函数：删除文档
    *  @return     void
    */
    void Slot_ActionDelNote();

    /**@fn         Slot_ActionRenameNote
    *  @brief      槽函数：重命名文档
    *  @return     void
    */
    void Slot_ActionRenameNote();

    /**@fn         Slot_UpdateDocNodeInfo
    *  @brief      槽函数：更新文档信息
    *  @para       QVariant var 文档信息
    *  @return     void
    */
    void Slot_UpdateDocNodeInfo(QVariant var);

signals:
    /**@fn         Signal_OpenDocFile
    *  @brief      信号： 打开文档
    *  @para       const QVariant& variant 变量，由RecordInfo转换，RecordInfo为记录信息
    *  @return     void
    */
    void Signal_OpenDocFile(const QVariant& variant);

    /**@fn         Signal_TreeNodeNameChanged
    *  @brief      信号： 重命名文档
    *  @para       const QString& qstrNewTreeNodeName 新的文档名称
    *  @return     void
    */
    void Signal_TreeNodeNameChanged(const QString& qstrNewTreeNodeName);

    /**@fn         Signal_SetEditDisableAndBlank
    *  @brief      信号： 设置文本编辑处内容为空且不可用
    *  @return     void
    */
    void Signal_SetEditDisableAndBlank();
};

#endif // DOCMANAGERWIDGET_H
