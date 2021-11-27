#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
    scene = new QGraphicsScene(this);
    this->setScene(scene);
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
    int x = mousePosition.x();
    int y = mousePosition.y();
    qDebug() << x;
    qDebug() << y;
    // test czy dodawanie gestosci zadziala
    simulation->AddSource(x, y, 0.2f);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    qDebug() << mousePosition.x();
    qDebug() << mousePosition.y();
}


void MyGraphicsView::start() {
    qDebug() << "Przycisk Dziala";
    timer->setInterval(this->interval);
    timer->start();
    simulation = new Simulation(this->height(), this->width(), (float)this->v->value(), (float)this->d->value());
}

void MyGraphicsView::refresh(){
    qDebug() << "dziala timer";
    float* pixels = (float*)malloc((this->height() + 4) * sizeof(float) * (this->width() + 4));
    simulation->GetNextFrame(pixels,interval);

    image = new QImage(this->height(), this->width(),QImage::Format_RGB888);
    for (int i = 0; i < this->width(); i++) {
        for (int j = 0; j < this->height(); j++) {
            image->setPixelColor(i, j, getColor(pixels[i + 2, j + 2]));
        }
    }
    scene->addPixmap(QPixmap::fromImage(*image));
    this->show();
}

QColor MyGraphicsView::getColor(float x) {
    if (x < 0)
        return QColor(0, 255, 0, 255);
    else {
        int y = (int)(x * 255);
        return QColor(y, y, y, 255);
    }
}

