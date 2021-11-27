#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
}

void MyGraphicsView::giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit *ts) 
{
    this->v = v;
    this->d = d;
    this->ts = ts;
    QString tmp = ts->toPlainText();
    QByteArray bytearray = tmp.toLocal8Bit();
    const char* string = bytearray.data();
    this->interval = atoi(string);
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


void MyGraphicsView::start() {
    timer->setInterval(this->interval);
    timer->start();
    simulation = new Simulation(this->height(), this->width(), (float)this->v->value(), (float)this->d->value());
}

void MyGraphicsView::refresh(){

    float* pixels = simulation->GetNextFrame(interval);
    QColor** colors = (QColor **) malloc(this->height() * sizeof(QColor *));
    for (int i = 0; i < this->height(); i++) {
        colors[i] = (QColor*)malloc(this->width() * sizeof(QColor));
    }

}

