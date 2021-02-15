

#include "DiaryManagerWidget.h"
#include "ui_DiaryManagerWidget.h"
#include "GlobalData.h"
#include "CommonDefine.h"
#include "DataManager.h"
#include <QDir>


CDiaryManagerWidget::CDiaryManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDiaryManagerWidget),
    m_pTreeModel(nullptr)
{
    ui->setupUi(this);
    ui->treeView_diary->setContextMenuPolicy(Qt::CustomContextMenu);

    {
        m_actionAdd = new QAction();
        m_actionAdd ->setObjectName("actionAdd");
        m_actionAdd ->setText(tr("添加子层日记"));
        connect(m_actionAdd, SIGNAL(triggered(bool)), this, SLOT(Slot_AddTreeNode()));

        m_actionDel = new QAction();
        m_actionDel ->setObjectName("actionDel");
        m_actionDel ->setText(tr("删除"));
        connect(m_actionDel, SIGNAL(triggered(bool)), this, SLOT(Slot_DelTreeNode()));

        m_actionRename = new QAction(&m_menuDiarySubNode);
        m_actionRename ->setObjectName("actionRename");
        m_actionRename ->setText(tr("重命名"));
        connect(m_actionRename, SIGNAL(triggered(bool)), this, SLOT(Slot_RenameTreeNode()));
    }

    {
        m_menuDiaryNode.addAction(m_actionAdd);
        m_menuDiaryNode.addAction(m_actionDel);

        m_menuDiarySubNode.addAction(m_actionAdd);
        m_menuDiarySubNode.addAction(m_actionDel);
        m_menuDiarySubNode.addAction(m_actionRename);
    }

    InitTreeData();

    connect(ui->calendarWidget,SIGNAL(clicked(const QDate&)), this, SLOT(Slot_CalendarClick(const QDate&)));
    connect(ui->treeView_diary, SIGNAL(clicked(const QModelIndex&)), this, SLOT(Slot_TreeViewClick(const QModelIndex&)));
    connect(ui->treeView_diary, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(Slot_ShowContextMenu(const QPoint&)));
    connect(m_pTreeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(Slot_ItemChanged(QStandardItem*)));
}

void CDiaryManagerWidget::InitTreeData()
{
    m_pTreeModel = new QStandardItemModel(ui->treeView_diary);//创建模型
    QStringList test;
    test.append("日记");
    m_pTreeModel->setHorizontalHeaderLabels(test);
    ui->treeView_diary->setModel(m_pTreeModel);//导入模型


    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    int iUserId = stuUserInfo.iId;
    std::list<RecordInfo> listRecordInfo;
    bool bRst = CDataManager::GetInstance()->DiaryDB_GetDiaryInfoByParentId(0, iUserId, listRecordInfo);//根节点id为0该表为日期节点，其他为日期节点的子节点
    if(false == bRst)
    {
        return;
    }

    for(auto element : listRecordInfo)
    {
        //QFileInfo fileInfo = list.at(i);
        //QString qstrFileName = fileInfo.fileName();//文件名格式为2020_08_09.txt
        QString qstrTreeNodeName = element.qstrTreeNodeName;
        qDebug("%s", qstrTreeNodeName.toStdString().c_str());

        //解析“年”
        int iPosFirstSeparator = qstrTreeNodeName.indexOf("_");
        QString qstrYear = qstrTreeNodeName.mid(0, iPosFirstSeparator);
        int iYear = qstrYear.toInt();

        //解析“月”
        int iPosSecondSeparator = qstrTreeNodeName.indexOf("_", iPosFirstSeparator + 1);
        QString qstrMonth = qstrTreeNodeName.mid(iPosFirstSeparator + 1, iPosSecondSeparator - iPosFirstSeparator - 1);
        int iMonth = qstrMonth.toInt();

        //解析“日”
        int iPosThirdSeparator = qstrTreeNodeName.indexOf(".");
        QString qstrDay = qstrTreeNodeName.mid(iPosSecondSeparator + 1, iPosThirdSeparator - iPosSecondSeparator - 1);
        int iDay = qstrDay.toInt();

        QDate date;
        date.setDate(iYear, iMonth, iDay);
        InsertTreeItem_Day(element.iId, element.iParentId, element.qstrTreeNodeName, element.qstrFilePath, date);
    }
}

CDiaryManagerWidget::~CDiaryManagerWidget()
{
    delete ui;
}

//点击事件
void CDiaryManagerWidget::Slot_TreeViewClick(const QModelIndex& modelIndex)
{
    QStandardItem* pItem =  m_pTreeModel->itemFromIndex(modelIndex);
    QVariant variantTypeOfDayOrSub = pItem->data(UserRole_IsDayOrSub);
    bool bTypeOfDayOrSub = variantTypeOfDayOrSub.value<bool>();
    if(true == bTypeOfDayOrSub)
    {
        OpenDiary(pItem);
//        RecordInfo stuRecordInfo;
//        stuRecordInfo.eType = RECORD_DIATY;
//        stuRecordInfo.iId = pItem->data(UserRole_Id).toInt();
//        stuRecordInfo.iParentId = pItem->data(UserRole_ParentId).toInt();
//        stuRecordInfo.qstrTreeNodeName = pItem->data(UserRole_NodeName).toString();
//        stuRecordInfo.qstrFilePath = pItem->data(UserRole_FilePath).toString();

//        QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
//        UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
//        stuRecordInfo.iUserId = stuUserInfo.iId;

//        QVariant variant;
//        variant.setValue(stuRecordInfo);
//        emit Signal_OpenDiaryFile(variant);
    }
    else
    {
        emit Signal_SetEditDisableAndBlank();
    }
}

void CDiaryManagerWidget::Slot_AddTreeNode()
{
    QModelIndex modelIndex = ui->treeView_diary->currentIndex();
    QStandardItem* pItem =  m_pTreeModel->itemFromIndex(modelIndex);
    int iParentId = pItem->data(UserRole_Id).toInt();


    QString qstrNodeName = "新建日记";
    QStandardItem* pItemChild = new QStandardItem(qstrNodeName);
    //pItemChild->
    pItemChild->setData(true, UserRole_IsDayOrSub);
    pItemChild->setData(-1, UserRole_Id);
    pItemChild->setData(iParentId, UserRole_ParentId);
    pItemChild->setData(qstrNodeName, UserRole_NodeName);
    pItem->appendRow(pItemChild);
    //
    OpenDiary(pItem);
}

void CDiaryManagerWidget::Slot_DelTreeNode()
{
    QModelIndex modelIndex = ui->treeView_diary->currentIndex();
    QStandardItem* pItem =  m_pTreeModel->itemFromIndex(modelIndex);
    if(nullptr == pItem)
    {
        qCritical("pItem is null");
        return;
    }
    int iId = pItem->data(UserRole_Id).toInt();
    if(nullptr != CDataManager::GetInstance())
    {
        CDataManager::GetInstance()->DiaryDB_DeleteDiary(iId);
    }
    else
    {
        qCritical("pDataManager is null");
    }

    int iRow = pItem->row();
    QModelIndex modelIndexParent = pItem->parent()->index();
    m_pTreeModel->removeRow(iRow, modelIndexParent);
}

void CDiaryManagerWidget::Slot_RenameTreeNode()
{
    QModelIndex modelIndex = ui->treeView_diary->currentIndex();
    QStandardItem* pItem =  m_pTreeModel->itemFromIndex(modelIndex);

    pItem->setEditable(true);
    ui->treeView_diary->edit(modelIndex);
}

void CDiaryManagerWidget::InsertTreeItem_Day(int iId, int iParentId, const QString& qstrNodeName, const QString& qstrFilePath, const QDate& date)
{
    do
    {
        if(nullptr == m_pTreeModel)
        {
            break;
        }

        int iYear = date.year();
        int iMonth = date.month();
        int iDay = date.day();

        if(iYear < 1900)
        {
            break;
        }


        QStandardItem* pItemYear = nullptr;
        QStandardItem* pItemMonth = nullptr;
        QStandardItem* pItemDay = nullptr;
        //查找年份应该插入的位置，并插入
        {
            QList<QStandardItem*> listYear = m_pTreeModel->findItems(QString::number(iYear) + "年");
            if(listYear.length() <= 0)
            {
                int iPosYear = 0;
                for(int i = 1900 ; i < iYear; ++i)
                {
                    QList<QStandardItem*> list = m_pTreeModel->findItems(QString::number(i) + "年");
                    if(list.length() > 0)
                    {
                        iPosYear++;
                    }
                }

                pItemYear = new QStandardItem(QString::number(iYear) + "年");
                pItemYear->setEditable(false);
                m_pTreeModel->insertRow(iPosYear, pItemYear);
            }
        }

        // 查找月份应该插入的位置，并插入
        {
            QList<QStandardItem*> listYear = m_pTreeModel->findItems(QString::number(iYear) + "年");
            pItemYear = listYear.at(0);

            QList<QStandardItem*> listMonth = m_pTreeModel->findItems(QString::number(iMonth) + "月", Qt::MatchRecursive);
            bool bFindMonth = false;
            for(int i = 0; i < listMonth.length(); ++i)
            {
                if(listMonth.at(i)->parent()->text() == pItemYear->text())
                {
                    bFindMonth = true;
                    pItemMonth = listMonth.at(i);
                    break;
                }
            }


            if(false == bFindMonth)
            {
                int iPosMonth = 0;
                for(int i = 1 ; i < iMonth; ++i)
                {
                    QList<QStandardItem*> list = m_pTreeModel->findItems(QString::number(i) + "月", Qt::MatchRecursive);
                    for(int j = 0; j < list.length(); ++j)
                    {
                        if(list.at(j)->parent()->text() == pItemYear->text())
                        {
                            iPosMonth++;
                            break;
                        }
                    }
                }

                pItemMonth = new QStandardItem(QString::number(iMonth) + "月");
                pItemMonth->setEditable(false);
                //pItemMonth->setIcon(QIcon(":/images/login/arrow_up.png"));
                pItemYear->insertRow(iPosMonth, pItemMonth);
            }
        }

        //查找日应该插入的位置，并插入
        {
            QList<QStandardItem*> listDay = m_pTreeModel->findItems(QString::number(iDay) + "日", Qt::MatchRecursive);
            bool bFindDay = false;
            for(int i = 0; i < listDay.length(); ++i)
            {
                if(listDay.at(i)->parent()->parent()->text() == pItemYear->text() && listDay.at(i)->parent()->text() == pItemMonth->text())
                {
                    pItemDay = listDay.at(i);
                    bFindDay = true;
                    break;
                }
            }

            if(false == bFindDay)
            {
                int iPosDay = 0;
                for(int i =  1; i < iDay; ++i)
                {
                    QList<QStandardItem*> listDay = m_pTreeModel->findItems(QString::number(i) + "日", Qt::MatchRecursive);
                    for(int i = 0; i < listDay.length(); ++i)
                    {
                        if(listDay.at(i)->parent()->parent()->text() == pItemYear->text() && listDay.at(i)->parent()->text() == pItemMonth->text())
                        {
                            iPosDay++;
                            break;
                        }
                    }
                }
                pItemDay = new QStandardItem(QString::number(iDay) + "日");
                pItemDay->setEditable(false);
                pItemDay->setData(true, UserRole_IsDayOrSub);
                pItemDay->setData(date, UserRole_Date);
                pItemDay->setData(iId, UserRole_Id);
                pItemDay->setData(iParentId, UserRole_ParentId);
                pItemDay->setData(qstrNodeName, UserRole_NodeName);
                pItemDay->setData(qstrFilePath, UserRole_FilePath);
                //pItemDay->setIcon(QIcon(":/images/login/arrow_up.png"));
                pItemMonth->insertRow(iPosDay, pItemDay);

                QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
                UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
                int iUserId = stuUserInfo.iId;
                std::list<RecordInfo> listRecord;
                CDataManager::GetInstance()->DiaryDB_GetDiaryInfoByParentId(iId, iUserId, listRecord);
                for(auto itor: listRecord)
                {
                    AddTreeItem_Sub(pItemDay, itor.iId, itor.iParentId, itor.qstrTreeNodeName, itor.qstrFilePath);
                }

            }
        }
    }while (0);
}

void CDiaryManagerWidget::AddTreeItem_Sub(QStandardItem *pParentItem, int iId, int iParentId, const QString &qstrNodeName, const QString &qstrFilePath)
{
    if(nullptr == pParentItem)
    {
        return;
    }
    QStandardItem* pItem = new QStandardItem(qstrNodeName);
    pItem->setEditable(false);
    pItem->setData(true, UserRole_IsDayOrSub);
    pItem->setData(iId, UserRole_Id);
    pItem->setData(iParentId, UserRole_ParentId);
    pItem->setData(qstrNodeName, UserRole_NodeName);
    pItem->setData(qstrFilePath, UserRole_FilePath);
    pParentItem->appendRow(pItem);

    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    int iUserId = stuUserInfo.iId;
    std::list<RecordInfo> listRecord;
    CDataManager::GetInstance()->DiaryDB_GetDiaryInfoByParentId(iId, iUserId, listRecord);
    for(auto itor: listRecord)
    {
        AddTreeItem_Sub(pItem, itor.iId, itor.iParentId, itor.qstrTreeNodeName, itor.qstrFilePath);
    }

}

void CDiaryManagerWidget::OpenDiary(const QStandardItem* pItem)
{
    RecordInfo stuRecordInfo;
    stuRecordInfo.eType = RECORD_DIATY;
    stuRecordInfo.iId = pItem->data(UserRole_Id).toInt();
    qInfo("lg_test OpenDiary %d", stuRecordInfo.iId);
    stuRecordInfo.iParentId = pItem->data(UserRole_ParentId).toInt();
    stuRecordInfo.qstrTreeNodeName = pItem->data(UserRole_NodeName).toString();
    stuRecordInfo.qstrFilePath = pItem->data(UserRole_FilePath).toString();

    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    stuRecordInfo.iUserId = stuUserInfo.iId;

    QVariant variant;
    variant.setValue(stuRecordInfo);
    emit Signal_OpenDiaryFile(variant);
}

void CDiaryManagerWidget::Slot_CalendarClick(const QDate &date)
{
    QString qstrTreeNodeName;
    qstrTreeNodeName.sprintf("%d_%d_%d", date.year(), date.month(), date.day());
    InsertTreeItem_Day(-1, 0, qstrTreeNodeName, "", date);
}

void CDiaryManagerWidget::Slot_ShowContextMenu(const QPoint& pos)
{
    QModelIndex modelIndex = ui->treeView_diary->currentIndex();
    QStandardItem* pItem =  m_pTreeModel->itemFromIndex(modelIndex);
    bool bTypeOfDayOrSub = pItem->data(UserRole_IsDayOrSub).value<bool>();
    int iParentId = pItem->data(UserRole_ParentId).value<int>();
    if(true == bTypeOfDayOrSub)
    {
        if(0 == iParentId)
        {
           m_menuDiaryNode.exec(ui->treeView_diary->mapToGlobal(pos)/*全局位置*/);
        }
        else
        {
            //! 显示该菜单，进入消息循环
            m_menuDiarySubNode.exec(ui->treeView_diary->mapToGlobal(pos)/*全局位置*/);
        }

    }
}

void CDiaryManagerWidget::Slot_ItemChanged(QStandardItem* pItem)
{
    int iId = pItem->data(UserRole_Id).toInt();
    QString qstrTreeNodeName = pItem->data(UserRole_NodeName).toString();
    if(iId > 0)
    {
        //更新树节点名称
        CDataManager::GetInstance()->DiaryDB_UpdateTreeNodeName(iId, qstrTreeNodeName);
    }
    else
    {
        //通知编辑界面树界面名称有修改
        Signal_TreeNodeNameChanged(qstrTreeNodeName);
    }
}

void CDiaryManagerWidget::Slot_UpateDiaryNodeInfo(QVariant var)
{
    RecordInfo stuRecordInfo = var.value<RecordInfo>();
    QModelIndex modelIndex = ui->treeView_diary->currentIndex();
    QStandardItem* pItem =  m_pTreeModel->itemFromIndex(modelIndex);
    pItem->setData(stuRecordInfo.iId, UserRole_Id);
    pItem->setData(stuRecordInfo.qstrFilePath, UserRole_FilePath);
}

