/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       GlobalData.h
*   Description:    全局数据
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#pragma execution_character_set("utf-8")
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QString>

class CGlobalData
{
public:
    CGlobalData();
    virtual ~CGlobalData();

    /**@fn         GetInstance
    *  @brief      获取单例对象
    *  @return     static CGlobalData* 单例对象
    */
    static CGlobalData* GetInstance()
    {
        static CGlobalData inst;
        return &inst;
    }

    /**@fn         Fun_SetUserName
    *  @brief      设置用户名
    *  @para       const QString& qstrUserName 用户名
    *  @return     void
    */
    void Fun_SetUserName(const QString& qstrUserName)
    {
        m_qstrUserName = qstrUserName;
    }

    /**@fn         Fun_GetUserName
    *  @brief      获取用户名
    *  @return     QString 用户名
    */
    QString Fun_GetUserName()
    {
        return m_qstrUserName;
    }

private:
    QString m_qstrUserName; //登陆用户名
};

#endif // GLOBALDATA_H
