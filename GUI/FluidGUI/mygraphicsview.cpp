#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{

}

void MyGraphicsView::mousePressEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();
    qDebug() <<  mousePosition.x();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    qDebug() << mousePosition.x();
}

