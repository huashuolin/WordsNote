/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       DiaryManagerWidget.h
*   Description:    管理日记目录
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef DIARYMANAGERWIDGET_H
#define DIARYMANAGERWIDGET_H
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>

namespace Ui {
class CDiaryManagerWidget;
}

class CDiaryManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDiaryManagerWidget(QWidget *parent = 0);
    virtual ~CDiaryManagerWidget();

private:
    Ui::CDiaryManagerWidget *ui;
    QStandardItemModel* m_pTreeModel; //日记目录树模型

    QMenu m_menuDiaryNode; //日记菜单(日记节点)
    QMenu m_menuDiarySubNode; //日记菜单(子节点)
    QAction* m_actionAdd; //添加日记
    QAction* m_actionDel; //删除日记
    QAction* m_actionRename; // 重命名

    /**@fn         InitTreeData
    *  @brief      初始化树（读取数据库中所有日记构建树结构）
    *  @return     void
    */
    void InitTreeData();

    /**@fn         InsertTreeItem_Day
    *  @brief      在日记树中加入节点(日记节点)
    *  @para       int iId 日期节点ID
    *  @para       int iParentId 父节点ID
    *  @para       const QString& qstrNodeName 树节点名称
    *  @para       const QString& qstrFilePath 文件存储路径
    *  @para       const QDate& date 日期
    *  @return     void
    */
    void InsertTreeItem_Day(int iId, int iParentId, const QString& qstrNodeName, const QString& qstrFilePath, const QDate& date);

    /**@fn         AddTreeItem_Sub
    *  @brief      在日记树中添加子节点
    *  @para       StandardItem* pParentItem 父节点
    *  @para       int iId 日期节点ID
    *  @para       int iParentId 父节点ID
    *  @para       const QString& qstrNodeName 树节点名称
    *  @para       const QString& qstrFilePath 文件存储路径
    *  @return     void
    */
    void AddTreeItem_Sub(QStandardItem* pParentItem, int iId, int iParentId, const QString& qstrNodeName, const QString& qstrFilePath);

    /**@fn         OpenDiary
    *  @brief      打开日记
    *  @para       const QStandardItem* pItem 日记树节点
    *  @return     void
    */
    void OpenDiary(const QStandardItem* pItem);

public slots:

    /**@fn         Slot_CalendarClick
    *  @brief      槽函数：日期控件点击事件
    *  @para       const QDate& date //日期
    *  @return     void
    */
    void Slot_CalendarClick(const QDate& date);

    /**@fn         Slot_TreeViewClick
    *  @brief      槽函数：日记树点击事件
    *  @para       const QModelIndex& modelIndex 树节点index
    *  @return     void
    */
    void Slot_TreeViewClick(const QModelIndex& modelIndex);


    /**@fn         Slot_AddTreeNode
    *  @brief      槽函数：添加日记
    *  @return     void
    */
    void Slot_AddTreeNode();

    /**@fn         Slot_DelTreeNode
    *  @brief      槽函数：删除日记
    *  @return     void
    */
    void Slot_DelTreeNode();

    /**@fn         Slot_RenameTreeNode
    *  @brief      槽函数：重命名日记
    *  @return     void
    */
    void Slot_RenameTreeNode();

    /**@fn         Slot_ShowContextMenu
    *  @brief      槽函数：展示右键菜单
    *  @para       const QPoint& pos 鼠标位置
    *  @return     void
    */
    void Slot_ShowContextMenu(const QPoint& pos);

    /**@fn         Slot_ItemChanged
    *  @brief      槽函数：日记树点击事件
    *  @para       QStandardItem* pItem 发生改变的树节点
    *  @return     void
    */
    void Slot_ItemChanged(QStandardItem* pItem);

    /**@fn         Slot_UpateDiaryNodeInfo
    *  @brief      槽函数：更新日记信息
    *  @para       QVariant var 日记信息
    *  @return     void
    */
    void Slot_UpateDiaryNodeInfo(QVariant var);

signals:
    /**@fn         Signal_OpenDiaryFile
    *  @brief      信号： 打开日记
    *  @para       const QVariant& variant 变量，由RecordInfo转换，RecordInfo为记录信息
    *  @return     void
    */
    void Signal_OpenDiaryFile(const QVariant& variant);

    /**@fn         Signal_SetEditDisableAndBlank
    *  @brief      信号： 设置文本编辑处内容为空且不可用
    *  @return     void
    */
    void Signal_SetEditDisableAndBlank();

    /**@fn         Signal_TreeNodeNameChanged
    *  @brief      信号： 重命名日记文档
    *  @para       const QString& qstrNewTreeNodeName 新的日记名称
    *  @return     void
    */
    void Signal_TreeNodeNameChanged(const QString& qstrNewTreeNodeName);

};

#endif // DIARYMANAGERWIDGET_H
