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
    //int x = mousePosition.x();
    //int y = mousePosition.y();
    ////qDebug() << x;
    ////qDebug() << y;
    //// test czy dodawanie gestosci zadziala
    //if (simulation2 != nullptr) {
    //    simulation2->AddDensity(x,y,1.0f);
    //}
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    int x1 = pressPosition.x() < mousePosition.x()  ? pressPosition.x() : mousePosition.x();
    int x2 = pressPosition.x() > mousePosition.x() ? pressPosition.x() : mousePosition.x();
    int y1 = pressPosition.y() < mousePosition.y() ? pressPosition.y() : mousePosition.y();
    int y2 = pressPosition.y() > mousePosition.y() ? pressPosition.y() : mousePosition.y();

    for (int j = y1; j <= y2; j++) {
        for (int i = x1; i <= x2; i++) {
            simulation2->AddDensity(i, j, 0.5f);
        }
    }
    e->button()
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
    //szerokość chyba jest większa niż rysowanie bo jest przesuniętę w kązdym rzędzie o kilka więcej pikxeli
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
    //bardzo niskie x kilka tysięcy poniżej 0
    //TCHAR s[32];
    //swprintf(s,32, __TEXT("Density is %f \n"), x);
    //OutputDebugString(s);
    if (x < 0)
        return QColor(0, 255, 0, 255);
    else {
        int y = (int)(x * 255);
        return QColor(y, y, y, 255);
    }
}

