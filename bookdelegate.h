#ifndef BOOKDELEGATE_H
#define BOOKDELEGATE_H

//委托类，与用户产生交互

//提供了一个用于显示和编辑来自QSqlRelationalTableModel的数据的委托。
#include <QSqlRelationalDelegate>

class BookDelegate : public QSqlRelationalDelegate
{
public:
    BookDelegate(QObject *parent);

    //绘制
    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const override;

    //考虑到选项提供的样式信息，返回委托显示index指定的项所需的大小。
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;

    //当开始编辑一个项时，将使用触发编辑的事件、模型、项的索引和用于呈现项的选项来调用该函数。即使鼠标事件没有开始编辑项，也会发送到editorEvent()。
    bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index) override;

    //返回用于编辑index指定的项的小部件。父部件和样式选项用于控制编辑器部件的显示方式。
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;

private:
    QPixmap star; //屏幕外的图像表示，可以用作绘制设备。存储等级的五角星

};

#endif // BOOKDELEGATE_H
