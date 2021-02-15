#pragma execution_character_set("utf-8")
#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H

#include <QString>
#include <QObject>

//记录类型
enum RECORD_TYPE
{
    RECORD_DIATY = 0, //日记
    RECORD_DOCUMENT //文档
};



#define UserRole_IsDayOrSub      (Qt::UserRole + 1)
#define UserRole_Date       (Qt::UserRole + 2)
#define UserRole_Id         (Qt::UserRole + 3)
#define UserRole_ParentId   (Qt::UserRole + 4)
#define UserRole_NodeName   (Qt::UserRole + 5)
#define UserRole_FilePath   (Qt::UserRole + 6)

//用户信息
typedef struct tag_UserInfo
{
    tag_UserInfo()
    {
        iId = -1;
    }

    int iId; //用户ID
    QString qstrName; //用户名
    QString qstrPwd; //密码
    QString qstrImagePath; //头像存储路径

    tag_UserInfo& operator=(const tag_UserInfo& other)
    {
        if(this != &other)
        {
            iId = other.iId;
            qstrName = other.qstrName;
            qstrPwd = other.qstrPwd;
            qstrImagePath = other.qstrImagePath;
        }
        return *this;
    }

}UserInfo;

//记录信息
typedef struct tag_RecordInfo
{
    tag_RecordInfo()
    {
        eType = RECORD_DIATY;
        iId = -1;
        iParentId = -1;
        iUserId = -1;
    }

    RECORD_TYPE eType; //记录类型
    int iId; //ID
    int iParentId; //父ID
    int iUserId; //用户ID
    QString qstrTreeNodeName; //树节点名称
    QString qstrFilePath; //日记或文档存储路径

    tag_RecordInfo& operator=(const tag_RecordInfo& other)
    {
        if(this != &other)
        {
            eType = other.eType;
            iId = other.iId;
            iUserId = other.iUserId;
            iParentId = other.iParentId;
            qstrTreeNodeName = other.qstrTreeNodeName;
            qstrFilePath = other.qstrFilePath;
        }
        return *this;
    }

}RecordInfo;
Q_DECLARE_METATYPE(RecordInfo)

#endif // COMMONDEFINE_H
