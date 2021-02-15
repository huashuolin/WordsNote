/**************************************************************************
*   Copyright       huashuolin001
*   FileName:       log.h
*   Description:    日志
*   Author:         huashuolin001
*   Date:           20/04/2020
*   Modification History:
*    <version>       <time>          <author>        <desc>
*       1.0        20/04/2020     huashuolin001       创建
**************************************************************************/

#ifndef LOG_H
#define LOG_H
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMutex>

#define LOGTXTNAME "WordsNoteLog.txt" //日志文件名称

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtInfoMsg:
        text = QString("Info");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        break;
    default:
        break;
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(current_date_time).arg(msg);
    QFile file(LOGTXTNAME);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

#endif // LOG_H
