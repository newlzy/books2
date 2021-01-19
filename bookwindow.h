#ifndef BOOKWINDOW_H
#define BOOKWINDOW_H

#include <QtWidgets>
#include <QtSql>

#include "ui_bookwindow.h"

class BookWindow : public QMainWindow
{
    Q_OBJECT
public:
     BookWindow();

private slots:
     void about();

private:
    void showError(const QSqlError &err); //显示错误状态
    Ui::BookWindow ui; //BookWindow是ui窗体QMainWindow的指针
    //为单个数据库表提供一个可编辑的数据模型
    QSqlRelationalTableModel *model;
    int authorIdx,genreIdx;

    void createMenuBar();

};

#endif // BOOKWINDOW_H
