#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
    simulation = new Simulation(this->height(), this->width());
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

void MyGraphicsView::intervalUpdate(int i) {
    interval = i;
}

void MyGraphicsView::start() {
    timer->setInterval(this->interval);
    timer->start();
}

void MyGraphicsView::refresh(){

    int** pixels = simulation->GetNextFrame(interval);
    QColor** colors = (QColor **) malloc(this->height() * sizeof(QColor *));
    for (int i = 0; i < this->height(); i++) {
        colors[i] = (QColor*)malloc(this->width() * sizeof(QColor));
    }

}

