/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       TextOperateWidget.h
*   Description:    文本操作窗口
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef TEXTOPERATEWIDGET_H
#define TEXTOPERATEWIDGET_H
#pragma execution_character_set("utf-8")

#include "CommonDefine.h"

#include <QWidget>
#include <QTextCharFormat>

namespace Ui {
class CTextOperateWidget;
}

class CTextOperateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CTextOperateWidget(QWidget *parent = 0);
    virtual ~CTextOperateWidget();

private:
    Ui::CTextOperateWidget *ui;

    QString m_qstrFileFullPath; //文件的全路径（包含文件名）
    QString m_qstrLastSaveData; //记录上次保存的数据（用于修改文件名（树节点名称）时，将上次保存的数据写入到新文件名称的文件中）
    RecordInfo m_stuRecordInfo; //当前记录信息

    /**@fn         DepriveParenthesesWithUUID
    *  @brief      去除自动生成UUID最外层的括号
    *  @para       const QString& qstrData 自动生成的UUID
    *  @return     QString 去除最外层括号的UUID
    */
    QString DepriveParenthesesWithUUID(const QString& qstrData);

    /**@fn         CreateFileFolder
    *  @brief      创建日记、文档存储的文件夹
    *  @return     void
    */
    void CreateFileFolder();

    /**@fn         SetDefaultFormat
    *  @brief      设置默认文本格式
    *  @return     void
    */
    void SetDefaultFormat();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);


public slots:

    /******************************工具栏函数 begin***********************************/
    /**@fn         Slot_FontChange
    *  @brief      槽函数：字体改变
    *  @para       QString qstrFont    字体
    *  @return     void
    */
    void Slot_FontChange(QString qstrFont);

    /**@fn         Slot_FontSizeChange
    *  @brief      槽函数：字体大小改变
    *  @para       QString qstrFontSize    字体大小
    *  @return     void
    */
    void Slot_FontSizeChange(QString qstrFontSize);

    /**@fn         Slot_TextBoldChange
    *  @brief      槽函数：粗体改变
    *  @return     void
    */
    void Slot_TextBoldChange();

    /**@fn         Slot_TextItalicChange
    *  @brief      槽函数：斜体改变
    *  @return     void
    */
    void Slot_TextItalicChange();

    /**@fn         Slot_TextItalicChange
    *  @brief      槽函数：下划线改变
    *  @return     void
    */
    void Slot_TextUnderlineChange();

    /**@fn         Slot_TextColorChange
    *  @brief      槽函数：文字颜色改变
    *  @return     void
    */
    void Slot_TextColorChange();

    /**@fn         Slot_TextBackgroundColorChange
    *  @brief      槽函数：文字背景颜色改变
    *  @return     void
    */
    void Slot_TextBackgroundColorChange();

    /**@fn         Slot_TextAlignLeft
    *  @brief      槽函数：左对齐
    *  @return     void
    */
    void Slot_TextAlignLeft();

    /**@fn         Slot_TextAlignCenter
    *  @brief      槽函数：中间对齐
    *  @return     void
    */
    void Slot_TextAlignCenter();

    /**@fn         Slot_TextAlignRight
    *  @brief      槽函数：右对齐
    *  @return     void
    */
    void Slot_TextAlignRight();

    /**@fn         Slot_TextAlignBothEnds
    *  @brief      槽函数：两端对齐
    *  @return     void
    */
    void Slot_TextAlignBothEnds();


    /**@fn         Slot_ShowCurrentFormatChange
    *  @brief      槽函数：将当前鼠标所在的文本格式显示在工具栏中
    *  @para       const QTextCharFormat& fmt 当前鼠标所在的文本格式
    *  @return     void
    */
    void Slot_ShowCurrentFormatChange(const QTextCharFormat& fmt);

    /******************************工具栏函数 end***********************************/


    /**@fn         Slot_OpenDiaryFile
    *  @brief      槽函数：打开日记文件
    *  @para       const QVariant& variant
    *  @return     void
    */
    void Slot_OpenDiaryFile(const QVariant& variant);

    /**@fn         Slot_OpenDocFile
    *  @brief      槽函数：打开文档文件
    *  @para       const QVariant& variant
    *  @return     void
    */
    void Slot_OpenDocFile(const QVariant& variant);


    /**@fn         Slot_TreeNodeNameChanged
    *  @brief      槽函数：名称改变（树节点名称改变）
    *  @para       const QString& qstrNewTreeNodeName    修改后的名称
    *  @return     void
    */
    void Slot_TreeNodeNameChanged(const QString& qstrNewTreeNodeName);

    /**@fn         Slot_SaveFile
    *  @brief      槽函数：保存到文件
    *  @return     void
    */
    void Slot_SaveFile();

    /**@fn         Slot_EditDisableAndBlank
    *  @brief      槽函数：将文本编辑框限制内容置为空且不可用
    *  @return     void
    */
    void Slot_EditDisableAndBlank();

signals:
    /**@fn         Signal_UpdateDiaryNodeInfo
    *  @brief      信息：更新日记信息
    *  @param      QVariant var 日记信息
    *  @return     void
    */
    void Signal_UpdateDiaryNodeInfo(QVariant var);

    /**@fn         Signal_UpdateDocumentNodeInfo
    *  @brief      信息：更新文档信息
    *  @param      QVariant var, 文档信息
    *  @return     void
    */
    void Signal_UpdateDocumentNodeInfo(QVariant var);
};

#endif // TEXTOPERATEWIDGET_H
