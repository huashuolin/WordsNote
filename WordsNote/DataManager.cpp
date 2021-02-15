#include "DataManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

CDataManager::CDataManager()
{

}

CDataManager::~CDataManager()
{

}

void CDataManager::Init()
{
    //建立并打开数据库
    m_database = QSqlDatabase::addDatabase("QSQLITE", "MyWordsNote");
    m_database.setHostName("WordsNote");
    m_database.setDatabaseName("WordsNote.db");
    m_database.setUserName("root");
    m_database.setPassword("Abc12345");

#ifdef _WIN32//Windows
    QSqlQuery::QSqlQuery(m_database);
#endif

    if (!m_database.open())
    {
        QSqlError error = m_database.lastError();
        qDebug("Error: Failed to connect database, %s", m_database.lastError().text().toStdString().c_str());
    }
    else
    {
        qDebug("Succeed to connect database") ;
    }

    if(false == JudgeTableExist("user"))
    {
        UserDB_CreateTable();
    }

    if(false == JudgeTableExist("diary"))
    {
        DiaryDB_CreateTable();
    }

    if(false == JudgeTableExist("document"))
    {
        DocumentDB_CreateTable();
    }
}

void CDataManager::UnInit()
{
    //关闭数据库
    if(m_database.isOpen())
    {
        m_database.close();
    }
    QSqlDatabase::removeDatabase("MyWordsNote");
}

bool CDataManager::JudgeTableExist(QString qstrTableName)
{
    QString qstrSQL;
    qstrSQL.sprintf("select * from sqlite_master where type = 'table' and name = '%s'", qstrTableName.toStdString().c_str());
    QSqlQuery sql_query(m_database);
    if(!sql_query.exec(qstrSQL))
    {
        qCritical("sql exec failed with select table");
        return false;
    }

    if(sql_query.next())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CDataManager::UserDB_CreateTable()
{
    QSqlQuery sql_query(m_database);
    if(!sql_query.exec("create table user(id int primary key, name text, password text, imagePath text)"))
    {
        qCritical("Error: Fail to create user table, %s", sql_query.lastError().text().toStdString().c_str());
    }
}

bool CDataManager::UserDB_AddUser(const QString& qstrUserName, const QString& qstrPwd, const QString& qstrImagePath)
{
    bool bRtn = false;
    do
    {
        int iMaxUserId = UserDB_GetMaxUserId();
        int iUserId = iMaxUserId + 1;

        QString qstrSQL;
        qstrSQL.sprintf("INSERT INTO user VALUES(%d, \"%s\", \"%s\", \"%s\")", iUserId, qstrUserName.toStdString().c_str(), qstrPwd.toStdString().c_str(), qstrImagePath.toStdString().c_str());
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to add user, %s", sql_query.lastError().text().toStdString().c_str());
        }
        bRtn = true;
    }while(0);
    return bRtn;
}

bool CDataManager::UserDB_VertifyUser(const QString& qstrUserName, const QString& qstrPwd)
{
    bool bRtn = false;
    do
    {
        QString qstrSQL;
        qstrSQL.sprintf("select * from user where name='%s'", qstrUserName.toStdString().c_str());
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to select user of %s, %s", qstrUserName.toStdString().c_str(), sql_query.lastError().text().toStdString().c_str());
        }
        else
        {
            while(sql_query.next())
            {
                QString qstrPwdDb  = sql_query.value("password").toString();
                if(qstrPwd == qstrPwdDb)
                {
                    bRtn = true;
                }
            }
        }
    }while(0);
    return bRtn;
}

int CDataManager::UserDB_GetMaxUserId()
{
    int iMaxUserId = -1;
    do
    {
        //查询数据
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec("select * from user"))
        {
            //qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                int iUserId  = sql_query.value("id").toInt();
                if(iUserId > iMaxUserId)
                {
                    iMaxUserId = iUserId;
                }
            }
        }
    }while(0);
    return iMaxUserId;
}

bool CDataManager::UserDB_IsUserExist(const QString& qstrUserName)
{
    bool bRtn = false;
    do
    {
        QSqlQuery sql_query(m_database);
        QString qstrSQL;
        qstrSQL.sprintf("select * from user where name='%s'", qstrUserName.toStdString().c_str());
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to select user of %s, %s", qstrUserName.toStdString().c_str(), sql_query.lastError().text().toStdString().c_str());
        }
        else
        {
            if(sql_query.next())
            {
                bRtn = true;
            }
        }
    }while(0);
    return bRtn;
}

UserInfo CDataManager::UserDB_GetUserInfoByUserName(const QString& qstrUserName)
{
    UserInfo stuUserInfo;
    QString qstrSQL;
    qstrSQL.sprintf("select * from user where name='%s'", qstrUserName.toStdString().c_str());
    QSqlQuery sql_query(m_database);
    if(!sql_query.exec(qstrSQL))
    {
        qCritical("Error: Fail to select user of %s, %s", qstrUserName.toStdString().c_str(), sql_query.lastError().text().toStdString().c_str());
    }
    else
    {
        if(sql_query.next())
        {
            stuUserInfo.iId = sql_query.value("id").toInt();
            stuUserInfo.qstrName = sql_query.value("name").toString();
            stuUserInfo.qstrPwd = sql_query.value("password").toString();
            stuUserInfo.qstrImagePath = sql_query.value("imagePath").toString();
        }
    }
    return stuUserInfo;
}

void CDataManager::DiaryDB_CreateTable()
{
    QSqlQuery sql_query(m_database);
    if(!sql_query.exec("create table diary(id int primary key, parentId int, userId int, treeNodeName text, filePath text)"))
    {
        qCritical("Error: Fail to create diary table, %s", sql_query.lastError().text().toStdString().c_str());
    }
}

bool CDataManager::DiaryDB_AddDiary(int iParentId, int iUserId, const QString &qstrTreeNodeName, const QString &qstrFilePath, int& iDiaryId)
{
    bool bRtn = false;
    do
    {
        int iMaxDiaryId = DiaryDB_GetMaxDiaryId();
        if(iMaxDiaryId < 0)
        {
            break;
        }

        int iId = iMaxDiaryId + 1;
        QString qstrSQL;
        qstrSQL.sprintf("insert into diary values(%d, %d, %d, \"%s\", \"%s\")", iId, iParentId, iUserId, qstrTreeNodeName.toStdString().c_str(), qstrFilePath.toStdString().c_str());
        //插入数据
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to insert %s into diary", qstrTreeNodeName.toStdString().c_str());
            break;
        }
        iDiaryId= iId;
        bRtn = true;
    }while(0);
    return bRtn;
}

bool CDataManager::DiaryDB_DeleteDiary(int iId)
{
    QSqlQuery sql_query(m_database);
    QString qstrSQL;
    qstrSQL.sprintf("delete from diary where id=%d", iId);
    if(!sql_query.exec(qstrSQL))
    {
        qCritical("Error: Fail to delete diary, id = %d", iId);
        return false;
    }
    return true;
}

int CDataManager::DiaryDB_GetMaxDiaryId()
{
    int iMaxDiaryId = 1;
    do
    {
        //查询数据
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec("select * from diary order by id desc"))
        {
            qCritical("select diary failed");
            break;
        }
        else
        {
            if(sql_query.next())
            {
                iMaxDiaryId  = sql_query.value("id").toInt();
            }
        }
    }while(0);
    return iMaxDiaryId;
}

bool CDataManager::DiaryDB_GetDiaryInfoByParentId(int iParentId, int iUserId, std::list<RecordInfo>& listRecordInfo)
{
    bool bRtn = false;
    do
    {
        //查询数据
        QString qstrSQL;
        qstrSQL.sprintf("select * from diary where parentId=%d and userId=%d", iParentId, iUserId);
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to get diary info");
            break;
        }
        else
        {
            while(sql_query.next())
            {
                RecordInfo stuRecordInfo;
                stuRecordInfo.eType = RECORD_DIATY;
                stuRecordInfo.iId = sql_query.value("id").toInt();
                stuRecordInfo.iParentId = sql_query.value("parentId").toInt();
                stuRecordInfo.iUserId = sql_query.value("userId").toInt();
                stuRecordInfo.qstrTreeNodeName = sql_query.value("treeNodeName").toString();
                stuRecordInfo.qstrFilePath = sql_query.value("filePath").toString();
                listRecordInfo.push_back(stuRecordInfo);
            }
        }
        bRtn = true;
    }while (0);
    return bRtn;
}

bool CDataManager::DiaryDB_GetDiaryInfoById(int iId, int iUserId, RecordInfo &stuRecordInfo)
{
    bool bRtn = false;
    do
    {
        //查询数据
        QString qstrSQL;
        qstrSQL.sprintf("select * from diary where id=%d and userId=%d", iId, iUserId);
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to get diary info");
            break;
        }
        else
        {
            if(sql_query.next())
            {
                stuRecordInfo.eType = RECORD_DIATY;
                stuRecordInfo.iId = sql_query.value("id").toInt();
                stuRecordInfo.iParentId = sql_query.value("parentId").toInt();
                stuRecordInfo.iUserId = sql_query.value("userId").toInt();
                stuRecordInfo.qstrTreeNodeName = sql_query.value("treeNodeName").toString();
                stuRecordInfo.qstrFilePath = sql_query.value("filePath").toString();
            }
        }
        bRtn = true;
    }while (0);
    return bRtn;
}

void CDataManager::DiaryDB_UpdateTreeNodeName(int iId, const QString& qstrTreeNoteName)
{
    QSqlQuery sql_query(m_database);
    QString qstrSQL;
    qstrSQL.sprintf("update diary set treeNodeName = '%s' where id = %d", qstrTreeNoteName.toStdString().c_str(), iId);
    if(!sql_query.exec(qstrSQL))
    {
        qCritical("Error: Fail to %s, %s", qstrSQL.toStdString().c_str(), sql_query.lastError().text().toStdString().c_str());
    }
}

void CDataManager::DocumentDB_CreateTable()
{
    QSqlQuery sql_query(m_database);
    if(!sql_query.exec("create table document(id int primary key, parentId int, userId int, treeNodeName text, filePath text)"))
    {
        qCritical("Error: Fail to create document table, %s", sql_query.lastError().text().toStdString().c_str());
    }
}

bool CDataManager::DocumentDB_AddDocument(int iParentId, int iUserId, const QString &qstrTreeNodeName, const QString &qstrFilePath, int& iDocId)
{
    bool bRtn = false;
    do
    {
        int iMaxDocumentId = DocumentDB_GetMaxDocumentId();
        if(iMaxDocumentId < 0)
        {
            break;
        }

        int iDocumentId = iMaxDocumentId + 1;
        QString qstrSQL;
        qstrSQL.sprintf("insert into document values(%d, %d, %d, \"%s\", \"%s\")", iDocumentId, iParentId, iUserId, qstrTreeNodeName.toStdString().c_str(), qstrFilePath.toStdString().c_str());
        //插入数据
        qInfo("lg_test %s", qstrSQL.toStdString().c_str());
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("insert document failed");
            break;
        }
        iDocId = iDocumentId;
        bRtn = true;
    }while(0);
    return bRtn;
}

bool CDataManager::DocumentDB_DeleteDocument(int iId)
{
    QSqlQuery sql_query(m_database);
    QString qstrSQL;
    qstrSQL.sprintf("delete from document where id=%d", iId);
    if(!sql_query.exec(qstrSQL))
    {
        qCritical("Error: Fail to delete document, id = %d", iId);
        return false;
    }
    return true;
}

int CDataManager::DocumentDB_GetMaxDocumentId()
{
    int iMaxDocumentId = 1;
    do
    {
        //查询数据
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec("select * from document order by id desc"))
        {
            qCritical("select document failed");
            break;
        }
        else
        {
            if(sql_query.next())
            {
                iMaxDocumentId  = sql_query.value("id").toInt();
            }
        }
    }while(0);
    return iMaxDocumentId;
}

bool CDataManager::DocumentDB_GetDocInfoByParentId(int iParentId, int iUserId, std::list<RecordInfo> &listRecordInfo)
{
    bool bRtn = false;
    do
    {
        //查询数据
        QString qstrSQL;
        qstrSQL.sprintf("select * from document where parentId=%d and userId=%d", iParentId, iUserId);
        QSqlQuery sql_query(m_database);
        if(!sql_query.exec(qstrSQL))
        {
            qCritical("Error: Fail to get document info");
            break;
        }
        else
        {
            while(sql_query.next())
            {
                RecordInfo stuRecordInfo;
                stuRecordInfo.eType = RECORD_DOCUMENT;
                stuRecordInfo.iId = sql_query.value("id").toInt();
                stuRecordInfo.iParentId = sql_query.value("parentId").toInt();
                stuRecordInfo.iUserId = sql_query.value("userId").toInt();
                stuRecordInfo.qstrTreeNodeName = sql_query.value("treeNodeName").toString();
                stuRecordInfo.qstrFilePath = sql_query.value("filePath").toString();
                listRecordInfo.push_back(stuRecordInfo);
            }
        }
        bRtn = true;
    }while (0);
    return bRtn;
}

void CDataManager::DocumentDB_UpdateTreeNodeName(int iId, const QString &qstrTreeNoteName)
{
    QSqlQuery sql_query(m_database);
    QString qstrSQL;
    qstrSQL.sprintf("update document set treeNodeName = '%s' where id = %d", qstrTreeNoteName.toStdString().c_str(), iId);
    if(!sql_query.exec(qstrSQL))
    {
        qCritical("Error: Fail to %s, %s", qstrSQL.toStdString().c_str(), sql_query.lastError().text().toStdString().c_str());
    }
}
