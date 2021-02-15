/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       DataManager.h
*   Description:    数据管理类，负责与数据库的交互，管理用户、日记、文档数据
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#pragma execution_character_set("utf-8")
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "CommonDefine.h"

#include <QString>
#include <QSqlDatabase>
#include <list>


class CDataManager
{
  public:
    CDataManager();
    virtual ~CDataManager();


    static CDataManager* GetInstance()
    {
        static CDataManager inst;
        return &inst;
    }

    /**@fn         Init
    *  @brief      初始化，创建数据库和表，打开数据库
    *  @return     void
    */
    void Init();

    /**@fn         UnInit
    *  @brief      反初始化，关闭数据库
    *  @return     void
    */
    void UnInit();

    /**@fn         JudgeTableExist
    *  @brief      判断表是否存在
    *  @para       QString qstrTableName    表名称
    *  @return     bool true代表成功，false代表失败
    */
    bool JudgeTableExist(QString qstrTableName);

    /*********************************用户表相关 begin****************************************/


    /**@fn         UserDB_CreateTable
    *  @brief      创建用户表
    *  @return     void
    */
    void UserDB_CreateTable();

    /**@fn         UserDB_AddUser
    *  @brief      添加用户
    *  @para       const QString& qstrUserName 用户名
    *  @para       const QString& qstrPwd 密码
    *  @para       const QString& qstrImagePath 用户头像存储路径
    *  @return     bool true代表添加成功，false代表添加失败
    */
    bool UserDB_AddUser(const QString& qstrUserName, const QString& qstrPwd, const QString& qstrImagePath);

    /**@fn         UserDB_VertifyUser
    *  @brief      验证用户名密码
    *  @para       const QString& qstrUserName 用户名
    *  @para       const QString& qstrPwd 密码
    *  @return     bool true代表验证成功，false代表验证失败
    */
    bool UserDB_VertifyUser(const QString& qstrUserName, const QString& qstrPwd);

    /**@fn         UserDB_IsUserExist
    *  @brief      判断用户是否存在
    *  @para       const QString& qstrUserName 用户名
    *  @return     bool true代表用户存在，false代表用户不存在
    */
    bool UserDB_IsUserExist(const QString& qstrUserName);

    /**@fn         UserDB_GetUserInfoByUserName
    *  @brief      根据用户名获取用户信息
    *  @para       const QString& qstrUserName 用户名
    *  @return     UserInfo 用户信息
    */
    UserInfo UserDB_GetUserInfoByUserName(const QString& qstrUserName);

    /**@fn         UserDB_GetMaxUserId
    *  @brief      获取当前最大的用户ID
    *  @return     int 当前最大用户ID
    */
    int UserDB_GetMaxUserId();

    /*********************************用户表相关 end****************************************/


    /*********************************日记表相关 begin****************************************/

    /**@fn         DiaryDB_CreateTable
    *  @brief      创建日记表
    *  @return     void
    */
    void DiaryDB_CreateTable();

    /**@fn         DiaryDB_AddDiary
    *  @brief      添加日记
    *  @para       int iParentId 父节点ID
    *  @para       int iUserId 用户ID
    *  @para       const QString& qstrTreeNodeName 树节点名称
    *  @para       const QString& qstrFilePath 文件存储路径
    *  @para       [OUT]int& iDiaryId 添加成功后返回的日记id值
    *  @return     bool true代表成功，false代表失败
    */
    bool DiaryDB_AddDiary(int iParentId, int iUserId, const QString& qstrTreeNodeName, const QString& qstrFilePath, int& iDiaryId);

    /**@fn         DiaryDB_DeleteDiary
    *  @brief      删除日记
    *  @para       int iId 日记ID
    *  @return     bool true代表成功，false代表失败
    */
    bool DiaryDB_DeleteDiary(int iId);

    /**@fn         DiaryDB_GetMaxDiaryId
    *  @brief      获取最大日记ID
    *  @return     int 最大日记ID
    */
    int DiaryDB_GetMaxDiaryId();

    /**@fn         DiaryDB_GetDiaryInfoByParentId
    *  @brief      通过父ID获取日记信息
    *  @para       int iParentId 父ID
    *  @para       int iUserId 用户ID
    *  @para       [OUT] std::list<RecordInfo>& listRecordInfo 日记列表
    *  @return     bool true代表成功，false代表失败
    */
    bool DiaryDB_GetDiaryInfoByParentId(int iParentId, int iUserId, std::list<RecordInfo>& listRecordInfo);

    /**@fn         DiaryDB_GetDiaryInfoById
    *  @brief      通过ID获取日记信息
    *  @para       int iId 日记ID
    *  @para       int iUserId 用户ID
    *  @para       [OUT] RecordInfo& stuRecordInfo 日记信息
    *  @return     bool true代表成功，false代表失败
    */
    bool DiaryDB_GetDiaryInfoById(int iId, int iUserId, RecordInfo& stuRecordInfo);

    /**@fn         DiaryDB_UpdateTreeNodeName
    *  @brief      更新树节点名称
    *  @para       int iId 日记ID
    *  @para       const QString& qstrTreeNoteName 树节点名称
    *  @return     void
    */
    void DiaryDB_UpdateTreeNodeName(int iId, const QString& qstrTreeNoteName);

    /*********************************日记表相关 end****************************************/

    /*********************************文档表相关 begin****************************************/


    /**@fn         DocumentDB_CreateTable
    *  @brief      创建文档表
    *  @return     void
    */
    void DocumentDB_CreateTable();

    /**@fn         DocumentDB_AddDocument
    *  @brief      添加文档
    *  @para       int iParentId 父ID
    *  @para       int iUserID 用户ID
    *  @para       const QString& qstrTreeNodeName 树节点名称
    *  @para       const QString& qstrFilePath 文档文件存储路径
    *  @para       [OUT]int& iDocId 添加成功后，文档的id
    *  @return     bool true代表成功，false代表失败
    */
    bool DocumentDB_AddDocument(int iParentId, int iUserID, const QString& qstrTreeNodeName, const QString& qstrFilePath, int& iDocId);

    /**@fn         DocumentDB_DeleteDocument
    *  @brief      删除文档
    *  @para       int iId 文档ID
    *  @return     bool true代表成功，false代表失败
    */
    bool DocumentDB_DeleteDocument(int iId);

    /**@fn         DocumentDB_GetMaxDocumentId
    *  @brief      获取当前最大文档ID
    *  @return     int 当前最大文档ID
    */
    int DocumentDB_GetMaxDocumentId();

    /**@fn         DocumentDB_GetDocInfoByParentId
    *  @brief      获取父ID下所有子文档
    *  @para       int iParentId 父ID
    *  @para       int iUserId 用户ID
    *  @para       [OUT]std::list<RecordInfo>& listRecordInfo 子文档列表
    *  @return     bool true代表成功，false代表失败
    */
    bool DocumentDB_GetDocInfoByParentId(int iParentId, int iUserId, std::list<RecordInfo>& listRecordInfo);

    /**@fn         DocumentDB_UpdateTreeNodeName
    *  @brief      更新树节点名称
    *  @para       int iId 文档ID
    *  @para       const QString& qstrTreeNoteName 树节点名称
    *  @return     bool true代表成功，false代表失败
    */
    void DocumentDB_UpdateTreeNodeName(int iId, const QString& qstrTreeNoteName);

    /*********************************文档表相关 end****************************************/


private:
    QSqlDatabase m_database;

};

#endif // DATAMANAGER_H
