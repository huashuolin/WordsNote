#include "DocManagerWidget.h"
#include "ui_DocManagerWidget.h"
#include "GlobalData.h"
#include "DataManager.h"

#include <QDir>

CDocManagerWidget::CDocManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDocManagerWidget),
    m_actionAdd(nullptr),
    m_actionDel(nullptr),
    m_actionReName(nullptr)
{
    ui->setupUi(this);
    ui->treeView_doc->setContextMenuPolicy(Qt::CustomContextMenu);

    {
        m_actionAdd = new QAction(&m_menu);
        m_actionAdd ->setObjectName("actionAdd");
        m_actionAdd ->setText(tr("添加子层日文档"));
        m_menu.addAction(m_actionAdd);
        connect(m_actionAdd, SIGNAL(triggered(bool)), this, SLOT(Slot_ActionAddNote()));
    }

    {
        m_actionDel = new QAction(&m_menu);
        m_actionDel ->setObjectName("actionDel");
        m_actionDel ->setText(tr("删除"));
        m_menu.addAction(m_actionDel);
        connect(m_actionDel, SIGNAL(triggered(bool)), this, SLOT(Slot_ActionDelNote()));
    }

    {
        m_actionReName = new QAction(&m_menu);
        m_actionReName ->setObjectName("actionRename");
        m_actionReName ->setText(tr("重命名"));
        m_menu.addAction(m_actionReName);
        connect(m_actionReName, SIGNAL(triggered(bool)), this, SLOT(Slot_ActionRenameNote()));
    }

    InitTreeData();

    connect(ui->toolButton_newDoc,SIGNAL(clicked()), this, SLOT(Slot_NewTreeNode()));
    connect(ui->treeView_doc, SIGNAL(clicked(const QModelIndex&)), this, SLOT(Slot_TreeViewClick(QModelIndex)));
    connect(m_pTreeModelDoc, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(Slot_ItemChanged(QStandardItem*)));


    connect(ui->treeView_doc, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(Slot_ShowContextMenu(const QPoint&)));
}

CDocManagerWidget::~CDocManagerWidget()
{
    delete ui;
}

void CDocManagerWidget::InitTreeData()
{


    m_pTreeModelDoc = new QStandardItemModel(ui->treeView_doc);//创建模型
    QStringList name;
    name.append("文档");
    m_pTreeModelDoc->setHorizontalHeaderLabels(name);
    ui->treeView_doc->setModel(m_pTreeModelDoc);//导入模型



    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    int iUserId = stuUserInfo.iId;
    std::list<RecordInfo> listRecord;
    CDataManager::GetInstance()->DocumentDB_GetDocInfoByParentId(0, iUserId, listRecord);
    for(auto itor: listRecord)
    {
        QStandardItem* pItem = new QStandardItem(itor.qstrTreeNodeName);
        pItem->setData(itor.iId, UserRole_Id);
        pItem->setData(itor.iParentId, UserRole_ParentId);
        pItem->setData(itor.qstrTreeNodeName, UserRole_NodeName);
        pItem->setData(itor.qstrFilePath, UserRole_FilePath);
        m_pTreeModelDoc->appendRow(pItem);

        std::list<RecordInfo> listRecord;
        CDataManager::GetInstance()->DocumentDB_GetDocInfoByParentId(itor.iId, iUserId, listRecord);
        for(auto itor2: listRecord)
        {
            AddTreeItem_Sub(pItem, itor2.iId, itor2.iParentId, itor2.qstrTreeNodeName, itor2.qstrFilePath);
        }
    }



    /*
    //构建文档文件的文件夹，遍历文件夹下的文件，构成树。暂定文件名以“树节点名称.data”的格式命名
    {
        QString qstrCurrentPath = QDir::currentPath();
        QString qstrDocFolder =  qstrCurrentPath + "/DocStorage";
        QDir dirDoc(qstrDocFolder);
        if(false == dirDoc.exists())
        {
            dirDoc.mkdir(qstrDocFolder);//只创建一级子目录，即必须保证上级目录存在
        }

        QFileInfoList list = dirDoc.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            QString qstrFileName = fileInfo.fileName();//文件名格式为2020_08_09.txt
            if(false == qstrFileName.contains(".txt"))
            {
                 continue;
            }
            qDebug("%s", qstrFileName.toStdString().c_str());

            //解析文件名
            int iPosSeparator = qstrFileName.indexOf(".");
            QString qstrName = qstrFileName.mid(0, iPosSeparator);

            m_pTreeModelDoc->appendRow(new QStandardItem(qstrName));
        }
    }
    */
}

void CDocManagerWidget::AddTreeItem_Sub(QStandardItem *pParentItem, int iId, int iParentId, const QString &qstrNodeName, const QString &qstrFilePath)
{
    if(nullptr == pParentItem)
    {
        return;
    }
    QStandardItem* pItem = new QStandardItem(qstrNodeName);
    pItem->setEditable(false);
    pItem->setData(iId, UserRole_Id);
    pItem->setData(iParentId, UserRole_ParentId);
    pItem->setData(qstrNodeName, UserRole_NodeName);
    pItem->setData(qstrFilePath, UserRole_FilePath);
    pParentItem->appendRow(pItem);

    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    int iUserId = stuUserInfo.iId;
    std::list<RecordInfo> listRecord;
    CDataManager::GetInstance()->DocumentDB_GetDocInfoByParentId(iId, iUserId, listRecord);
    for(auto itor: listRecord)
    {
        AddTreeItem_Sub(pItem, itor.iId, itor.iParentId, itor.qstrTreeNodeName, itor.qstrFilePath);
    }

}

void CDocManagerWidget::OpenDoc(const QStandardItem* pItem)
{
    RecordInfo stuRecordInfo;
    stuRecordInfo.eType = RECORD_DOCUMENT;
    stuRecordInfo.iId = pItem->data(UserRole_Id).toInt();
    stuRecordInfo.iParentId = pItem->data(UserRole_ParentId).toInt();
    stuRecordInfo.qstrTreeNodeName = pItem->data(UserRole_NodeName).toString();
    stuRecordInfo.qstrFilePath = pItem->data(UserRole_FilePath).toString();

    QString qstrUserName = CGlobalData::GetInstance()->Fun_GetUserName();
    UserInfo stuUserInfo = CDataManager::GetInstance()->UserDB_GetUserInfoByUserName(qstrUserName);
    stuRecordInfo.iUserId = stuUserInfo.iId;

    QVariant variant;
    variant.setValue(stuRecordInfo);
    emit Signal_OpenDocFile(variant);
}

void CDocManagerWidget::Slot_NewTreeNode()
{
    QString qstrNodeName = "新建文档";
    QStandardItem* pItem = new QStandardItem(qstrNodeName);
    pItem->setData(-1, UserRole_Id);
    pItem->setData(0, UserRole_ParentId);
    pItem->setData(qstrNodeName, UserRole_NodeName);
    m_pTreeModelDoc->appendRow(pItem);
    OpenDoc(pItem);
}

void CDocManagerWidget::Slot_TreeViewClick(const QModelIndex& modelIndex)
{
    //获取树节点的文件名
    QStandardItem* pItem =  m_pTreeModelDoc->itemFromIndex(modelIndex);
    OpenDoc(pItem);
}

void CDocManagerWidget::Slot_ItemChanged(QStandardItem *pItem)
{

    /*
    QString qstrName = pItem->text();
    emit Signal_DocNameChanged(qstrName);
    */
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

void CDocManagerWidget::Slot_ShowContextMenu(const QPoint& pos)
{
    //! 显示该菜单，进入消息循环
    m_menu.exec(ui->treeView_doc->mapToGlobal(pos)/*全局位置*/);
}

void CDocManagerWidget::Slot_ActionAddNote()
{
    QModelIndex modelIndex = ui->treeView_doc->currentIndex();
    QStandardItem* pItem =  m_pTreeModelDoc->itemFromIndex(modelIndex);

    int iParentId = pItem->data(UserRole_Id).toInt();
    QString qstrNodeName = "新建文档";
    QStandardItem* pItemChild = new QStandardItem(qstrNodeName);
    pItemChild->setData(-1, UserRole_Id);
    pItemChild->setData(iParentId, UserRole_ParentId);
    pItemChild->setData(qstrNodeName, UserRole_NodeName);
    pItem->appendRow(pItemChild);
    OpenDoc(pItem);
}

void CDocManagerWidget::Slot_ActionDelNote()
{
    QModelIndex modelIndex = ui->treeView_doc->currentIndex();
    QStandardItem* pItem =  m_pTreeModelDoc->itemFromIndex(modelIndex);
    int iRow = pItem->row();
    QModelIndex modelIndexParent = pItem->parent()->index();
    m_pTreeModelDoc->removeRow(iRow, modelIndexParent);

    int iId = pItem->data(UserRole_Id).toInt();
    CDataManager::GetInstance()->DocumentDB_DeleteDocument(iId);
}

void CDocManagerWidget::Slot_ActionRenameNote()
{
    QModelIndex modelIndex = ui->treeView_doc->currentIndex();
    QStandardItem* pItem =  m_pTreeModelDoc->itemFromIndex(modelIndex);

    pItem->setEditable(true);
    ui->treeView_doc->edit(modelIndex);
}

void CDocManagerWidget::Slot_UpdateDocNodeInfo(QVariant var)
{
    RecordInfo stuRecordInfo = var.value<RecordInfo>();
    QModelIndex modelIndex = ui->treeView_doc->currentIndex();
    QStandardItem* pItem =  m_pTreeModelDoc->itemFromIndex(modelIndex);
    pItem->setData(stuRecordInfo.iId, UserRole_Id);
    pItem->setData(stuRecordInfo.qstrFilePath, UserRole_FilePath);
}


