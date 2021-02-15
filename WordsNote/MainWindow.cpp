#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QColorDialog>
#include <QColor>
#include <QTextList>

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
    , m_pTreeModel(nullptr)
{

    ui->setupUi(this);

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮

    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小




    setWindowTitle(tr("Easy Word"));


    //在工具栏上嵌入控件
    //设置字体
    fontLabel1 =new QLabel(tr("字体:"));
    fontComboBox =new QFontComboBox;
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);

    fontLabel2 =new QLabel(tr("字号:"));
    sizeComboBox =new QComboBox;
    QFontDatabase db;
    foreach(int size,db.standardSizes())
        sizeComboBox->addItem(QString::number(size));

    boldBtn =new QToolButton;//加粗
    boldBtn->setIcon(QIcon("bold.png"));
    boldBtn->setCheckable(true);
    italicBtn =new QToolButton;//斜体
    italicBtn->setIcon(QIcon("italic.png"));
    italicBtn->setCheckable(true);

    underlineBtn =new QToolButton;//下划线
    underlineBtn->setIcon(QIcon("underline.png"));
    underlineBtn->setCheckable(true);

    colorBtn =new QToolButton;//颜色
    colorBtn->setIcon(QIcon("color.png"));
    colorBtn->setCheckable(true);

    //排序
    listLabel =new QLabel(tr("排序"));
    listComboBox =new QComboBox;
    listComboBox->addItem("Standard");
    listComboBox->addItem("QTextListFormat::ListDisc");
    listComboBox->addItem("QTextListFormat::ListCircle");
    listComboBox->addItem("QTextListFormat::ListSquare");
    listComboBox->addItem("QTextListFormat::ListDecimal");
    listComboBox->addItem("QTextListFormat::ListLowerAlpha");
    listComboBox->addItem("QTextListFormat::ListUpperAlpha");
    listComboBox->addItem("QTextListFormat::ListLowerRoman");
    listComboBox->addItem("QTextListFormat::ListUpperRoman");

    m_pTreeModel = new QStandardItemModel(ui->treeView);//创建模型
    ui->treeView->setModel(m_pTreeModel);//导入模型

    createActions();
    createMenus();
    createToolBars();

    connect(fontComboBox,SIGNAL(activated(QString)),this,SLOT(ShowFontComboBox(QString)));
    connect(sizeComboBox,SIGNAL(activated(QString)),this,SLOT(ShowSizeSpinBox(QString)));
    connect(boldBtn,SIGNAL(clicked()),this,SLOT(ShowBoldBtn()));
    connect(italicBtn,SIGNAL(clicked()),this,SLOT(ShowItalicBtn()));
    connect(underlineBtn,SIGNAL(clicked()),this,SLOT(ShowUnderlineBtn()));
    connect(colorBtn,SIGNAL(clicked()),this,SLOT(ShowColorBtn()));
    connect(ui->textEdit_record,SIGNAL(currentCharFormatChanged(QtextCharFormat&)),this,SLOT(ShowCurrentFormatChanged(QTextCharFormat&)));

    connect(listComboBox,SIGNAL(activated(int)),this,SLOT(ShowList(int)));
    connect(ui->textEdit_record->document(),SIGNAL(undoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(ui->textEdit_record->document(),SIGNAL(redoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(ui->textEdit_record,SIGNAL(cursorPositionChanged()),this,SLOT(ShowCursorPositionChanged()));

    //m_pShowWidget->GetData();
    ui->treeView->header()->hide();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->calendarWidget,SIGNAL(clicked(const QDate&)), this, SLOT(CalendarClick(const QDate&)));
    connect(ui->treeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(TreeViewClick_1(const QModelIndex&)));
    connect(ui->treeView, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(TreeViewClick_2(const QModelIndex&)));
    connect(ui->treeView, SIGNAL(expanded(const QModelIndex&)), this, SLOT(TreeViewClick_3(const QModelIndex&)));

}

CMainWindow::~CMainWindow()
{
    //m_pShowWidget->WriteData();
    delete ui;
}

void CMainWindow::createActions()
{
    //"复制"动作
    copyAction =new QAction(QIcon("copy.png"),tr("复制"),this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("复制文件"));
    connect(copyAction,SIGNAL(triggered()),ui->textEdit_record,SLOT(copy()));

    //"剪切"动作
    cutAction =new QAction(QIcon("cut.png"),tr("剪切"),this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("剪切文件"));
    connect(cutAction,SIGNAL(triggered()),ui->textEdit_record,SLOT(cut()));

    //"粘贴"动作
    pasteAction =new QAction(QIcon("paste.png"),tr("粘贴"),this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("粘贴文件"));
    connect(pasteAction,SIGNAL(triggered()),ui->textEdit_record,SLOT(paste()));


    //排序:左对齐、右对齐、居中和两端对齐
    actGrp =new QActionGroup(this);

    leftAction =new QAction(QIcon("left.png"),"左对齐",actGrp);
    leftAction->setCheckable(true);

    rightAction =new QAction(QIcon("right.png"),"右对齐",actGrp);
    rightAction->setCheckable(true);

    centerAction =new QAction(QIcon("center.png"),"居中",actGrp);
    centerAction->setCheckable(true);

    justifyAction =new QAction(QIcon("justify.png"),"两端对齐",actGrp);
    justifyAction->setCheckable(true);

    connect(actGrp,SIGNAL(triggered(QAction*)),this,SLOT(ShowAlignment(QAction*)));

    //实现撤销和重做的动作（Action）
    //撤销和重做
    undoAction =new QAction(QIcon("undo.png"),"撤销",this);
    connect(undoAction,SIGNAL(triggered()),ui->textEdit_record,SLOT(undo()));
    redoAction =new QAction(QIcon("redo.png"),"重做",this);
    connect(redoAction,SIGNAL(triggered()),ui->textEdit_record,SLOT(redo()));
}

void CMainWindow::createMenus()
{
    //缩放菜单
    zoomMenu =menuBar()->addMenu(tr("编辑"));
    zoomMenu->addAction(copyAction);
    zoomMenu->addAction(cutAction);
    zoomMenu->addAction(pasteAction);
    zoomMenu->addAction(aboutAction);
    zoomMenu->addSeparator();
}

void CMainWindow::createToolBars()
{
    //文件工具条
    fileTool =addToolBar("File");

    //编辑工具条
    zoomTool =addToolBar("Edit");
    zoomTool->addAction(copyAction);
    zoomTool->addAction(cutAction);
    zoomTool->addAction(pasteAction);
    zoomTool->addSeparator();

    //撤销和重做工具条
    doToolBar =addToolBar("doEdit");
    doToolBar->addAction(undoAction);
    doToolBar->addAction(redoAction);

    //字体工具条
    fontToolBar =addToolBar("Font");
    fontToolBar->addWidget(fontLabel1);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontLabel2);
    fontToolBar->addWidget(sizeComboBox);
    fontToolBar->addSeparator();
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addSeparator();
    fontToolBar->addWidget(colorBtn);

    //排序工具条
    listToolBar =addToolBar("list");
    listToolBar->addWidget(listLabel);
    listToolBar->addWidget(listComboBox);
    listToolBar->addSeparator();
    listToolBar->addActions(actGrp->actions());
}

void CMainWindow::ShowNewFile()
{
    CMainWindow *newCMainWindow =new CMainWindow;
    newCMainWindow->show();
}

void CMainWindow::ShowOpenFile()
{
    fileName =QFileDialog::getOpenFileName(this,"打开");
    if(!fileName.isEmpty())
    {
        if(ui->textEdit_record->document()->isEmpty())
        {
            loadFile(fileName);
        }
        else
        {
            CMainWindow *newCMainWindow =new CMainWindow;
            newCMainWindow->show();
            newCMainWindow->loadFile(fileName);
        }
    }
}

void CMainWindow::loadFile(QString filename)
{
    printf("file name:%s\n",filename.data());

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&file);
        while(!textStream.atEnd())
        {
            ui->textEdit_record->append(textStream.readLine());
            printf("read line\n");
        }
        printf("end\n");
    }
}

void CMainWindow::ShowPrintText()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec())
    {
        QTextDocument *doc = ui->textEdit_record->document();
        doc->print(&printer);
    }
}

void CMainWindow::ShowPrintImage()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec())
    {
        QPainter painter(&printer);
        QRect rect =painter.viewport();
        QSize size = img.size();
        size.scale(rect.size(),Qt::KeepAspectRatio);

        painter.setViewport(rect.x(),rect.y(),size.width(),size.height());
        painter.setWindow(img.rect());
        painter.drawImage(0,0,img);
    }
}

void CMainWindow::ShowZoomIn()
{
    if(img.isNull())
        return;
    QMatrix martix;
    martix.scale(2,2);
    img = img.transformed(martix);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowZoomOut()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.scale(0.5,0.5);
    img = img.transformed(matrix);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowRotate90()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.rotate(90);
    img = img.transformed(matrix);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowRotate180()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.rotate(180);
    img = img.transformed(matrix);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowRotate270()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.rotate(270);
    img = img.transformed(matrix);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowMirrorVertical()
{
    if(img.isNull())
        return;
    img=img.mirrored(false,true);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowMirrorHorizontal()
{
    if(img.isNull())
        return;
    img=img.mirrored(true,false);
    //m_pShowWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void CMainWindow::ShowFontComboBox(QString comboStr)		//设置字体
{
    QTextCharFormat fmt;
    fmt.setFontFamily(comboStr);
    mergeFormat(fmt);     					//把新的格式应用到光标选区内的字符
}

void CMainWindow::mergeFormat(QTextCharFormat format)
{
    QTextCursor cursor =ui->textEdit_record->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit_record->mergeCurrentCharFormat(format);
}

void CMainWindow::ShowSizeSpinBox(QString spinValue)       //设置字号
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(spinValue.toFloat());
    ui->textEdit_record->mergeCurrentCharFormat(fmt);
}

void CMainWindow::ShowBoldBtn()                            //设置文字显示加粗
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldBtn->isChecked()?QFont::Bold:QFont::Normal);
    ui->textEdit_record->mergeCurrentCharFormat(fmt);
}

void CMainWindow::ShowItalicBtn()                          //设置文字显示斜体
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicBtn->isChecked());
    ui->textEdit_record->mergeCurrentCharFormat(fmt);
}

void CMainWindow::ShowUnderlineBtn()                       //设置文字加下画线
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineBtn->isChecked());
    ui->textEdit_record->mergeCurrentCharFormat(fmt);
}

void CMainWindow::ShowColorBtn()                           //设置文字颜色
{
    QColor color=QColorDialog::getColor(Qt::red,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        ui->textEdit_record->mergeCurrentCharFormat(fmt);
    }
}

void CMainWindow::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
    fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
    sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
    boldBtn->setChecked(fmt.font().bold());
    italicBtn->setChecked(fmt.fontItalic());
    underlineBtn->setChecked(fmt.fontUnderline());
}

void CMainWindow::ShowAlignment(QAction *act)
{
    if(act==leftAction)
        ui->textEdit_record->setAlignment(Qt::AlignLeft);
    if(act==rightAction)
        ui->textEdit_record->setAlignment(Qt::AlignRight);
    if(act==centerAction)
        ui->textEdit_record->setAlignment(Qt::AlignCenter);
    if(act==justifyAction)
        ui->textEdit_record->setAlignment(Qt::AlignJustify);
}

void CMainWindow::ShowCursorPositionChanged()
{
    if(ui->textEdit_record->alignment()==Qt::AlignLeft)
        leftAction->setChecked(true);
    if(ui->textEdit_record->alignment()==Qt::AlignRight)
        rightAction->setChecked(true);
    if(ui->textEdit_record->alignment()==Qt::AlignCenter)
        centerAction->setChecked(true);
    if(ui->textEdit_record->alignment()==Qt::AlignJustify)
        justifyAction->setChecked(true);
}

void CMainWindow::ShowList(int index)
{
    QTextCursor cursor=ui->textEdit_record->textCursor();

    if(index!=0)
    {
        QTextListFormat::Style style=QTextListFormat::ListDisc;
        switch(index)                           //设置style属性值
        {
        default:
        case 1:
            style=QTextListFormat::ListDisc; break;
        case 2:
            style=QTextListFormat::ListCircle; break;
        case 3:
            style=QTextListFormat::ListSquare; break;
        case 4:
            style=QTextListFormat::ListDecimal; break;
        case 5:
            style=QTextListFormat::ListLowerAlpha; break;
        case 6:
            style=QTextListFormat::ListUpperAlpha; break;
        case 7:
            style=QTextListFormat::ListLowerRoman; break;
        case 8:
            style=QTextListFormat::ListUpperRoman; break;
        }
        cursor.beginEditBlock();                //设置缩进值

        QTextBlockFormat blockFmt=cursor.blockFormat();
        QTextListFormat listFmt;

        if(cursor.currentList())
        {
            listFmt= cursor.currentList()->format();
        }
        else
        {
            listFmt.setIndent(blockFmt.indent()+1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);

        cursor.endEditBlock();
        }
    else
    {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void CMainWindow::TreeViewClick_1(const QModelIndex&)
{
    int i = 0;
    ++i;
}

void CMainWindow::TreeViewClick_2(const QModelIndex&)
{
    int i = 0;
    ++i;
}

void CMainWindow::TreeViewClick_3(const QModelIndex&)
{
    int i = 0;
    ++i;
}

void CMainWindow::CalendarClick(const QDate &date)
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

                m_pTreeModel->insertRow(iPosYear, new QStandardItem(QString::number(iYear) + "年"));
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
                        }
                    }
                }

                pItemMonth = new QStandardItem(QString::number(iMonth) + "月");
                pItemYear->insertRow(iPosMonth, pItemMonth);
            }
        }

        //ui->treeView->ex

        //查找日应该插入的位置，并插入



//        QStandardItem * item = new QStandardItem("item one");//创建一个条目对象
//        m_pTreeModel->appendRow(item);//通过模型对象添加这个条目
//        QStandardItem * item2 = new QStandardItem(QString::fromLocal8Bit("第二条"));//创建一个条目对象
//        m_pTreeModel->appendRow(item2);//通过模型对象添加这个条目



    //    model->setItem(0,0,new QStandardItem(tr("item two")));
    //    model->setItem(2,0,new QStandardItem(tr("item three")));

    //    model->item(0)->appendRow(new QStandardItem(tr("item four")));
    }while (0);


}
