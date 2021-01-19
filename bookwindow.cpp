#include "bookwindow.h"
#include "initdb.h"
#include "bookdelegate.h"
#include <QtSql>
//#include <QMessageBox>
#pragma execution_character_set("utf-8")

//窗体布局
BookWindow::BookWindow()
{
    ui.setupUi(this); //在自身窗体内布局

    //drivers返回所有可用数据库驱动程序的列表。
    if(!QSqlDatabase::drivers().contains("QMYSQL")){
        QMessageBox::critical(this,"无法加载数据库！","需要驱动");
    }

    //QSqlError类提供SQL数据库错误信息。
    QSqlError err = initDb(); //连接数据库
    if(err.type() != QSqlError::NoError){
        showError(err); //数据库连接失败时
        return;
    }

    //为单个数据库表提供一个可编辑的数据模型。表视图
    model = new QSqlRelationalTableModel(ui.bookTable);
    //所有的更改都将缓存到模型中，直到submitAll()或revertAll()被调用
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("books"); //设置模型要操作的数据库表

    //fieldIndex返回字段的索引。获取作者和类型
    authorIdx = model->fieldIndex("author");
    genreIdx = model->fieldIndex("genre");

    //参数1的列由外键参数2来指定。QSqlRelation存储外键信息。authors是作者表
    model->setRelation(authorIdx,QSqlRelation("authors","id","name"));
    model->setRelation(genreIdx,QSqlRelation("genres","id","name"));

    //设置指定角色的水平标题
    model->setHeaderData(authorIdx,Qt::Horizontal,tr("作者名"));
    model->setHeaderData(genreIdx,Qt::Horizontal,tr("类型"));
    model->setHeaderData(model->fieldIndex("title"),Qt::Horizontal,tr("标题"));
    model->setHeaderData(model->fieldIndex("year"),Qt::Horizontal,tr("年份"));
    model->setHeaderData(model->fieldIndex("rating"),Qt::Horizontal,tr("等级"));

    //select使用通过setTable()设置的表中的数据填充模型
    if(!model->select()){
        showError(model->lastError());
        return;
    }

    ui.bookTable->setModel(model); //视图
    ui.bookTable->setItemDelegate(new BookDelegate(ui.bookTable)); //设置委托
    ui.bookTable->setColumnHidden(model->fieldIndex("id"),true);//隐藏指定的列
    //设置视图的选中模式。当用户选择项时，任何已经选择的项都将取消选择。
    ui.bookTable->setSelectionMode(QAbstractItemView::SingleSelection);

    //作者列表。设置模型.relationModel用于访问列为外键的表。
    ui.authorEdit->setModel(model->relationModel(authorIdx));
    //此属性保存模型中可见的列。作者列中的所有值
    ui.authorEdit->setModelColumn(model->relationModel(authorIdx)->fieldIndex("name"));

    //类型列表。relationModel外键
    ui.genreEdit->setModel(model->relationModel(genreIdx));
    ui.genreEdit->setModelColumn(model->relationModel(genreIdx)->fieldIndex("name"));

    //设置等级列的大小。表视图的水平表头。设置头文件的大小如何调整到给定模式所描述的大小的约束。QHeaderView将根据整个列或行的内容自动调整section的大小为其最佳大小。
    ui.bookTable->horizontalHeader()->setSectionResizeMode(model->fieldIndex("rating"),QHeaderView::ResizeToContents);

    //提供了数据模型的一部分到小部件之间的映射。
    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new BookDelegate(this)); //委托为自定义的
    mapper->addMapping(ui.titleEdit,model->fieldIndex("title")); mapper->addMapping(ui.yearEdit, model->fieldIndex("year"));
    mapper->addMapping(ui.authorEdit, authorIdx);
    mapper->addMapping(ui.genreEdit, genreIdx);
    mapper->addMapping(ui.ratingEdit, model->fieldIndex("rating"));

    //selectionModel返回当前选择模型。如果当前项发生更改时发出信号。将当前索引设置为索引的行
    connect(ui.bookTable->selectionModel(),&QItemSelectionModel::currentRowChanged,mapper,&QDataWidgetMapper::setCurrentModelIndex);

    //setCurrentIndex:将当前项设置为索引处的项。
    ui.bookTable->setCurrentIndex(model->index(0,0));


}

//显示数据库连接失败的错误信息
void BookWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this,"无法初始化数据库","误差初始化数据库:"+err.text());
}

//
void BookWindow::createMenuBar()
{
    //QAction插入到小部件中的抽象用户界面
    QAction *quitAction = new QAction(tr("&退出"),this);
    QAction *aboutAction = new QAction(tr("&简介"), this);
    QAction *aboutQtAction = new QAction(tr("&Qt 简介"), this);

    //菜单部件
    QMenu *fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(quitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&帮助"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    connect(quitAction, &QAction::triggered, this, &BookWindow::close);
    connect(aboutAction, &QAction::triggered, this, &BookWindow::about);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void BookWindow::about()
{
    QMessageBox::about(this, tr("关于此项目"),
            tr("演示如何使用 Qt SQL 类与模型/视图框架。"));
}
