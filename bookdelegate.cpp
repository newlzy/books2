#include "bookdelegate.h"
#include <QtWidgets>

BookDelegate::BookDelegate(QObject *parent):QSqlRelationalDelegate(parent),star(QPixmap(":/images/star.png"))
{

}

void BookDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //返回模型索引所引用的列。
    if(index.column() != 5){
        //用于在视图小部件中绘制项目的参数
        QStyleOptionViewItem opt = option;
        //adjust将坐标分别添加的现有矩阵的坐标。因为我们自己绘制网格
        opt.rect.adjust(0,0,-1,-1); //这个改动会怎样
        QSqlRelationalDelegate::paint(painter,opt,index);
    }else { //第五列
        //为项目模型提供了抽象接口
        const QAbstractItemModel *model = index.model();
        //QPalette包含每个小部件状态的颜色组。State_Enabled小部件是否以启用。State_Active小部件是活动的。
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ? (option.state & QStyle::State_Active)? QPalette::Normal: QPalette::Inactive: QPalette::Disabled;

        //是否有选择小部件
        if(option.state & QStyle::State_Selected){
            //被选中的小部件矩形颜色高亮
            painter->fillRect(option.rect,option.palette.color(cg,QPalette::Highlight));
        }

        //获取指定索引下的数据。等级
        int rating = model->data(index,Qt::DisplayRole).toInt();
        int width = star.width(); //五角星宽度
        int height = star.height();
        int x = option.rect.x();
        int y = option.rect.y() + (option.rect.height() / 2) - (height / 2);
        for(int i = 0; i < rating; ++i){
            painter->drawPixmap(x,y,star); //绘制五角星图片
            x += width; //有多少颗星星
        }
        //使用给定的绘制器和样式选项在rect指定的矩形内呈现区域
        drawFocus(painter,option,option.rect.adjusted(0,0,-1,-1));
    }

    QPen pen = painter->pen(); //当前的画笔
    painter->setPen(option.palette.color(QPalette::Mid)); //黑
    //绘制一条线。bottomLeft返回矩形的左下角位置，bottomRight右下角。
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    //左上角和右上角
    painter->drawLine(option.rect.topRight(),option.rect.bottomRight());
    painter->setPen(pen); //设置画笔
}


QSize BookDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 5)
        return QSize(5*star.width(),star.height()) + QSize(1,1);

    return QSqlRelationalDelegate::sizeHint(option,index) + QSize(1,1);
}

//编辑时。点击任意一单元格都会选中整行
bool BookDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() != 5) //不是点的第五列时
        return QSqlRelationalDelegate::editorEvent(event,model,option,index);

    //事件类型为鼠标按下
    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        //stars返回以最小值和最大值为界的val值。结果为5。应该是如果没数据就一列都不选中，有数据就选中5列
        int stars = qBound(0,int(0.7 + qreal(mouseEvent->pos().x() - option.rect.x()) / star.width()),5);
        model->setData(index,QVariant(stars));
    }
}

//选中第四列时，用旋转小部件来控制值。
QWidget *BookDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() != 4)
        return QSqlRelationalDelegate::createEditor(parent,option,index);

    QSpinBox *sb = new QSpinBox(parent); //旋转小部件
    sb->setFrame(false); //不用框架绘制自身，因为用parent
    sb->setMaximum(2021);
    sb->setMaximum(100);
    return sb;
}
