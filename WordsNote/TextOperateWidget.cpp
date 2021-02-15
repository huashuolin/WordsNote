#include "TextOperateWidget.h"
#include "ui_TextOperateWidget.h"
#include "GlobalData.h"
#include "DataManager.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDate>
#include <QColorDialog>
#include <QFontDatabase>
#include <QUuid>
#include <QKeyEvent>


CTextOperateWidget::CTextOperateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTextOperateWidget)
{
    ui->setupUi(this);

    ui->textEdit->setDisabled(true);

    installEventFilter(this);

    CreateFileFolder();

    //设置字体大小组
    QFontDatabase db;
    foreach(int size,db.standardSizes())
    {
        ui->comboBox_fontSize->addItem(QString::number(size));
    }

    //设置字体组
    ui->fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);//lg_test???


    connect(ui->fontComboBox,SIGNAL(activated(QString)),this,SLOT(Slot_FontChange(QString)));
    connect(ui->comboBox_fontSize,SIGNAL(activated(QString)),this,SLOT(Slot_FontSizeChange(QString)));
    connect(ui->toolButton_bold, SIGNAL(clicked()), this, SLOT(Slot_TextBoldChange()));
    connect(ui->toolButton_italic, SIGNAL(clicked()), this, SLOT(Slot_TextItalicChange()));
    connect(ui->toolButton_underline, SIGNAL(clicked()), this, SLOT(Slot_TextUnderlineChange()));

    connect(ui->toolButton_fontColor, SIGNAL(clicked()), this, SLOT(Slot_TextColorChange()));
    connect(ui->toolButton_fontBackgroundColor, SIGNAL(clicked()), this, SLOT(Slot_TextBackgroundColorChange()));

    connect(ui->toolButton_alignLeft, SIGNAL(clicked()),this, SLOT(Slot_TextAlignLeft()));
    connect(ui->toolButton_alignCenter, SIGNAL(clicked()),this, SLOT(Slot_TextAlignCenter()));
    connect(ui->toolButton_alignRight, SIGNAL(clicked()),this, SLOT(Slot_TextAlignRight()));
    connect(ui->toolButton_alignBothEnds, SIGNAL(clicked()),this, SLOT(Slot_TextAlignBothEnds()));

    connect(ui->toolButton_backward, SIGNAL(clicked()), ui->textEdit, SLOT(undo()));
    connect(ui->toolButton_forward, SIGNAL(clicked()), ui->textEdit, SLOT(redo()));

    connect(ui->toolButton_save, SIGNAL(clicked()), this, SLOT(Slot_SaveFile()));
    connect(ui->textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)), this, SLOT(Slot_ShowCurrentFormatChange(const QTextCharFormat&)));
}

CTextOperateWidget::~CTextOperateWidget()
{
    delete ui;
}

QString CTextOperateWidget::DepriveParenthesesWithUUID(const QString &qstrData)
{
    QString qstrRtn;
    do
    {
        int iPosFirst = qstrData.indexOf('{');
        int iPosLast = qstrData.indexOf('}');
        if(iPosFirst < 0 || iPosLast < 0 || iPosFirst > iPosLast)
        {
            break;
        }

        qstrRtn = qstrData.mid(iPosFirst + 1, iPosLast - iPosFirst - 1);
    }while(0);
    return qstrRtn;
}

void CTextOperateWidget::CreateFileFolder()
{
    QString qstrCurrentPath = QDir::currentPath();

    QString qstrFolder = qstrCurrentPath + "/DocStorage";
    QDir dir;
    if(!dir.exists(qstrFolder))
    {
        dir.mkdir(qstrFolder);
    }

    qstrFolder = qstrCurrentPath + "/DiaryStorage";
    if(!dir.exists(qstrFolder))
    {
        dir.mkdir(qstrFolder);
    }
}

void CTextOperateWidget::SetDefaultFormat()
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(12);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

bool CTextOperateWidget::eventFilter(QObject *watched, QEvent *event)
{
    do
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = (QKeyEvent*)event;
            if((keyEvent->key() == Qt::Key_S) && (keyEvent->modifiers() == Qt::ControlModifier))//Key_Control Key_S
            {
                Slot_SaveFile();
            }
        }
    }while(0);

    return QWidget::eventFilter(watched, event);
}

void CTextOperateWidget::Slot_TextBoldChange()                            //设置文字显示加粗
{
    QTextCharFormat fmt;
    fmt.setFontWeight(ui->toolButton_bold->isChecked()?QFont::Bold:QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void CTextOperateWidget::Slot_TextItalicChange()                          //设置文字显示斜体
{
    QTextCharFormat fmt;
    fmt.setFontItalic(ui->toolButton_italic->isChecked());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void CTextOperateWidget::Slot_TextUnderlineChange()                       //设置文字加下画线
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->toolButton_underline->isChecked());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void CTextOperateWidget::Slot_TextColorChange()
{
    QColor color=QColorDialog::getColor(Qt::red,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

void CTextOperateWidget::Slot_TextBackgroundColorChange()
{
    QColor color = QColorDialog::getColor(Qt::red,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setBackground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

void CTextOperateWidget::Slot_TextAlignLeft()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}

void CTextOperateWidget::Slot_TextAlignCenter()
{
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void CTextOperateWidget::Slot_TextAlignRight()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}

void CTextOperateWidget::Slot_TextAlignBothEnds()
{
    ui->textEdit->setAlignment(Qt::AlignJustify);
}

void CTextOperateWidget::Slot_SaveFile()
{
    if(m_stuRecordInfo.iId <= 0)
    {
        QUuid uuid = QUuid::createUuid();
        QString strUUID = uuid.toString();
        QString qstrUUIDWithoutParenttheses = DepriveParenthesesWithUUID(strUUID);
        QString qstrCurrentPath = QDir::currentPath();
        bool bAddRst = false;
        int iId = -1;
        if(RECORD_DOCUMENT == m_stuRecordInfo.eType)
        {
            m_stuRecordInfo.qstrFilePath = qstrCurrentPath + "/DocStorage/" + qstrUUIDWithoutParenttheses + ".wnd";
            // 保存到数据库
            qDebug("lg_test DocumentDB_AddDocument");
            bAddRst = CDataManager::GetInstance()->DocumentDB_AddDocument(m_stuRecordInfo.iParentId, m_stuRecordInfo.iUserId, m_stuRecordInfo.qstrTreeNodeName, m_stuRecordInfo.qstrFilePath, iId);
            if(true == bAddRst)
            {
                QVariant variant;
                m_stuRecordInfo.iId = iId;
                variant.setValue(m_stuRecordInfo);
                emit Signal_UpdateDocumentNodeInfo(variant);
            }
        }
        else if(RECORD_DIATY == m_stuRecordInfo.eType)
        {
            m_stuRecordInfo.qstrFilePath = qstrCurrentPath + "/DiaryStorage/" + qstrUUIDWithoutParenttheses + ".wnd";
            // 保存到数据库
            qDebug("lg_test DiaryDB_AddDiary");
            bAddRst = CDataManager::GetInstance()->DiaryDB_AddDiary(m_stuRecordInfo.iParentId, m_stuRecordInfo.iUserId, m_stuRecordInfo.qstrTreeNodeName, m_stuRecordInfo.qstrFilePath, iId);
            if(true == bAddRst)
            {
                QVariant variant;
                m_stuRecordInfo.iId = iId;
                variant.setValue(m_stuRecordInfo);
                emit Signal_UpdateDiaryNodeInfo(variant);
            }
        }
    }



    // 保存文件
    QFile file(m_stuRecordInfo.qstrFilePath);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        //file.write(ui->textEdit->toHtml().toStdString().c_str());
        //file.write(ui->textEdit->document()->toRawText().toStdString().c_str());

        QTextStream out(&file);
        out<<ui->textEdit->toHtml(); //转化成纯文本
        m_qstrLastSaveData = ui->textEdit->toHtml();
        file.close();
    }
}

void CTextOperateWidget::Slot_EditDisableAndBlank()
{
    ui->textEdit->setText("");
    ui->textEdit->setDisabled(true);
}

void CTextOperateWidget::Slot_TreeNodeNameChanged(const QString &qstrNewTreeNodeName)
{
    m_stuRecordInfo.qstrTreeNodeName = qstrNewTreeNodeName;
}

void CTextOperateWidget::Slot_OpenDiaryFile(const QVariant& variant)
{
    m_stuRecordInfo = variant.value<RecordInfo>();

    ui->textEdit->setDisabled(false);
    ui->textEdit->setText("");
    SetDefaultFormat();

    if(m_stuRecordInfo.iId > 0)
    {
        //m_qstrFileFullPath = m_stuRecordInfo.qstrFilePath;
        //判断文件是否存在。如果存在则读取并显示；如果不存在，则显示空内容
        QFileInfo fileInfo(m_stuRecordInfo.qstrFilePath);
        if(fileInfo.isFile())
        {
            //QString fileName = m_qstrFileFullPath;
            QFile file(m_stuRecordInfo.qstrFilePath);
            if(file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QTextStream in(&file);
                ui->textEdit->setText(in.readAll());//读取该文件的所有内容
                file.close();//关闭文件
            }
        }
    }
}

void CTextOperateWidget::Slot_OpenDocFile(const QVariant& variant)
{
    m_stuRecordInfo = variant.value<RecordInfo>();

    ui->textEdit->setDisabled(false);
    ui->textEdit->setText("");
    SetDefaultFormat();

    if(m_stuRecordInfo.iId > 0)
    {
        //m_qstrFileFullPath = m_stuRecordInfo.qstrFilePath;
        //判断文件是否存在。如果存在则读取并显示；如果不存在，则显示空内容
        QFileInfo fileInfo(m_stuRecordInfo.qstrFilePath);
        if(fileInfo.isFile())
        {
            //QString fileName = m_qstrFileFullPath;
            QFile file(m_stuRecordInfo.qstrFilePath);
            if(file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QTextStream in(&file);
                ui->textEdit->setText(in.readAll());//读取该文件的所有内容
                file.close();//关闭文件
            }
        }
    }
}

//void CTextOperateWidget::Slot_DocNameChanged(const QString& qstrName)
//{
//    //第一步：将上次保存的内容写入到新文件名称的文件中
//    {
//        QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
//        int iIndex = m_qstrFileFullPath.lastIndexOf("/");
//        QString qstrFilePath = m_qstrFileFullPath.mid(0, iIndex);
//        QString qstrNewFileFullPath = qstrFilePath + "/" + qstrUserName + "/" + qstrName + ".txt";

//        QFile file(qstrNewFileFullPath);
//        if(file.open(QIODevice::ReadWrite|QIODevice::Text))
//        {

//            QTextStream out(&file);
//            out<<m_qstrLastSaveData;
//            file.close();

//            //第二步：删除旧文件
//            {
//                QFile file(m_qstrFileFullPath);
//                file.remove();
//                m_qstrFileFullPath = qstrNewFileFullPath;
//            }
//        }
//    }
//}

void CTextOperateWidget::Slot_FontChange(QString qstrFont)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(qstrFont);
    //mergeFormat(fmt);

    QTextCursor cursor =ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void CTextOperateWidget::Slot_FontSizeChange(QString qstrFontSize)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(qstrFontSize.toFloat());
    //qInfo("lg_test FontSize : %ld", qstrFontSize.toFloat());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void CTextOperateWidget::Slot_ShowCurrentFormatChange(const QTextCharFormat& fmt)
{
    ui->fontComboBox->setCurrentIndex(ui->fontComboBox->findText(fmt.fontFamily()));
    ui->comboBox_fontSize->setCurrentIndex(ui->comboBox_fontSize->findText(QString::number(fmt.fontPointSize())));
    ui->toolButton_bold->setChecked(fmt.font().bold());
    ui->toolButton_italic->setChecked(fmt.fontItalic());
    ui->toolButton_underline->setChecked(fmt.fontUnderline());
}
