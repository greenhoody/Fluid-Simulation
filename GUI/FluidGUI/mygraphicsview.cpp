#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    pixels = (float*)malloc(((unsigned long long)this->height() + 4) * sizeof(float) * ((unsigned long long)this->width() + 4));
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
    image = new QImage(this->width(), this->height(), QImage::Format_RGB888);
    pixmap = new QPixmap(this->width(), this->height());
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
    //qDebug() << x;
    //qDebug() << y;
    // test czy dodawanie gestosci zadziala
    if (simulation != nullptr) {
        simulation->AddSource(x, y, 0.2f);
    }
 
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    //qDebug() << mousePosition.x();
    //qDebug() << mousePosition.y();
}


void MyGraphicsView::start() {
    //qDebug() << "Przycisk Dziala";
    simulation = new Simulation(this->height(), this->width(), (float)this->v->value(), (float)this->d->value());
    timer->setInterval(this->interval);
    timer->start();
}

void MyGraphicsView::refresh(){
    //tutaj wycieka mi pamiêæ bo zawsze nowy obraz daje
    //qDebug() << "dziala timer";
    simulation->GetNextFrame(pixels, this->interval);

    for (int i = 0; i < this->width(); i++) {
        for (int j = 0; j < this->height(); j++) {
            //image->setPixelColor(j, i, getColor(pixels[i + 2, j + 2]));
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

