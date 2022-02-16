#include "windows.h"
#include "debugapi.h"

#include "mygraphicsview.h"

#include <QMouseEvent>
#include <QDebug>

#define IX(i,j) ((i)+(simulation2->size+2)*(j))

MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
    //for some resone inside contructor size is diffrenet than after object is complete created
    //qDebug() << this->height();
    //qDebug() << this->width();
}

void MyGraphicsView::giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit *ts) 
{
    // it's must be here because for now its best thing to get values from thes widgets in code 
    this->v = v;
    this->d = d;
    this->ts = ts;
}

void MyGraphicsView::mousePressEvent(QMouseEvent * e){
    pressPosition = e->pos();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    int x1 = pressPosition.x() < mousePosition.x()  ? pressPosition.x() : mousePosition.x();
    int x2 = pressPosition.x() > mousePosition.x() ? pressPosition.x() : mousePosition.x();
    int y1 = pressPosition.y() < mousePosition.y() ? pressPosition.y() : mousePosition.y();
    int y2 = pressPosition.y() > mousePosition.y() ? pressPosition.y() : mousePosition.y();

    //boundary, not adding density to opposite side if coursor is far enough
    x1 = x1 < 0 ? 0 : x1;
    y1 = y1 < 0 ? 0 : y1;
    x2 = x2 > simulation2->size ? simulation2->size : x2;
    y2 = y2 > simulation2->size ? simulation2->size : y2;

    for (int j = y1; j <= y2; j++) {
        for (int i = x1; i <= x2; i++) {
            simulation2->AddDensity(i, j, 0.7f);
        }
    }
}


void MyGraphicsView::start() {
    QString tmp = ts->toPlainText();
    QByteArray bytearray = tmp.toLocal8Bit();
    const char* string = bytearray.data();
    this->interval = atoi(string);

    //Its must be here and not in constructor because this->width() and this->height() in constractor return wrong size
    pixels = (float*)malloc(sizeof(float) * (this->width() + 2) * (this->width() + 2));
    image = new QImage(this->width(), this->height(), QImage::Format_RGB888);
    pixmap = QPixmap(this->width(), this->height());
    scene = new QGraphicsScene(this);
    pixMapItem = scene->addPixmap(pixmap);
    this->setScene(scene);
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    if (simulation2 != NULL) {
        simulation2->FreeSimulation2();
        free(simulation2);
    }
    float diff = ((float)d->value()) / 100;
    float visc = ((float)v->value()) / 100;
    simulation2 = new Simulation2(this->width(), diff, visc, ((float)this->interval)/1000);
    timer->setInterval(this->interval);
    timer->start();
}

void MyGraphicsView::refresh(){
    simulation2->NextFrame(pixels);

    for (int i = 1; i < this->width() ; i++) {
        for (int j = 1; j < this->height() ; j++) {
            image->setPixelColor(i - 1, j - 1, getColor(pixels[IX(i, j)]));
        }
    }
    pixMapItem->setPixmap(QPixmap::fromImage(*image));
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

