#include "mygraphicsview.h"
#include <QMouseEvent>
#include <QDebug>



MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    pixels = (float*)malloc(((unsigned long long)this->height() + 4) * sizeof(float) * ((unsigned long long)this->width() + 4));
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
    //for some resone inside contructor size is diffrenet than intendet
    //qDebug() << this->height();
    //qDebug() << this->width();


}

void MyGraphicsView::giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit *ts) 
{
    // i'ts must be here because for now its best thing to get values from thes widgets in code 
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
    //Its must be here and not in constructor because this->width() and this->height() in constractor return wrong size
    image = new QImage(this->width(), this->height(), QImage::Format_RGB888);
    pixmap = QPixmap(this->width(), this->height());
    scene = new QGraphicsScene(this);
    pixMapItem = scene->addPixmap(pixmap);
    this->setScene(scene);
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    simulation = new Simulation(this->height(), this->width(), (float)this->v->value(), (float)this->d->value());
    //qDebug() << this->height();
    //qDebug() << simulation->simulator->height;
    //qDebug() << simulation->simulator->height2;
    //qDebug() << image->height();
    //qDebug() << this->width();
    //qDebug() << simulation->simulator->width;
    //qDebug() << simulation->simulator->width2;
    //qDebug() << image->width();

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

