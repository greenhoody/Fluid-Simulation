#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    timer = new QTimer(this);
}

void MyGraphicsView::mousePressEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();
    qDebug() <<  mousePosition.x();
    qDebug() << mousePosition.y();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    qDebug() << mousePosition.x();
    qDebug() << mousePosition.y();
}

