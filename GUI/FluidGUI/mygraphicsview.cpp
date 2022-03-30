#include "windows.h"
#include "debugapi.h"
#include "mygraphicsview.h"

//#include "../Factory/Simulation.h"
#include "../Factory/Factory.h"
#include "../Factory/FactoryNotEditedSimulation.h"
#include "../Factory/NotEditedSimulation.h"
#include "../Factory//FactoryEditedSimulation.h"
#include "../Factory//FactoryMA.h"


#include <QMouseEvent>
#include <QDebug>

#define IX(i,j) ((i)+(simulation->size+2)*(j))

//working 
constexpr auto SPEED_SCALE = 3.0f;
constexpr auto SPEED_CHANGE_RADIUS = 2;

MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
    //for some resone inside contructor size is diffrenet than after object is complete created
    //qDebug() << this->height();
    //qDebug() << this->width();
}

void MyGraphicsView::giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit* it, QPlainTextEdit* ft, QPlainTextEdit* den, QPlainTextEdit* hs, QPlainTextEdit* vs, QComboBox* kind)
{
    // it's must be here because for now its best thing to get values from thes widgets in code 
    this->v = v;
    this->d = d;
    this->it = it;
    this->ft = ft;
    this->den = den;
    this->hs = hs;
    this->vs = vs;
    this->kind = kind;
}

void MyGraphicsView::mousePressEvent(QMouseEvent * e){
    pressPosition = e->pos();
    lastPosition = e->pos();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * e){
    QPoint mousePosition = e->pos();

    int x1 = pressPosition.x();
    int x2 = mousePosition.x();
    int y1 = pressPosition.y();
    int y2 = mousePosition.y();

    if (e->button() == Qt::LeftButton) {
        switch (e->modifiers()) {
        //dodanie stałej gęstości
        case Qt::ControlModifier: {
            float constantDenisty = den->toPlainText().toFloat();
            for (int j = y1; j <= y2; j++) {
                for (int i = x1; i <= x2; i++) {
                    simulation->AddConstantDensity(i, j, constantDenisty);
                }
            }
        }
            break;
        // dodanie stałej prędkości
        case Qt::AltModifier: {
                float vSpeed = vs->toPlainText().toFloat();
                float hSpeed = hs->toPlainText().toFloat();
                for (int j = y1; j <= y2; j++) {
                    for (int i = x1; i <= x2; i++) {
                        simulation->AddConstantVelocity(i, j, vSpeed, hSpeed);
                    }
                }
            }
            break;

        // dodanie normalnej gęstości
        default:
            simulation->AddDensity(x1,x2,y1,y2,0.7f);
        }
    }
    else if (e->button() == Qt::RightButton) {
        switch (e->modifiers()) {
        case Qt::ControlModifier: {
            for (int j = y1; j <= y2; j++) {
                for (int i = x1; i <= x2; i++) {
                    simulation->DeleteConstantDensity(i, j);
                }
            }
        }
            break;
            // dodanie stałej prędkości
        case Qt::AltModifier:
            for (int j = y1; j <= y2; j++) {
                for (int i = x1; i <= x2; i++) {
                    simulation->DeleteConstantVelocity(i, j);
                }
            }
            break;
        }
    }

    if (e->button() == Qt::MiddleButton) {
        int x1 = pressPosition.x() < mousePosition.x() ? pressPosition.x() : mousePosition.x();
        int x2 = pressPosition.x() > mousePosition.x() ? pressPosition.x() : mousePosition.x();
        int y1 = pressPosition.y() < mousePosition.y() ? pressPosition.y() : mousePosition.y();
        int y2 = pressPosition.y() > mousePosition.y() ? pressPosition.y() : mousePosition.y();

        //boundary, not adding density to opposite side if coursor is far enough
        x1 = x1 < 0 ? 0 : x1;
        y1 = y1 < 0 ? 0 : y1;
        x2 = x2 > simulation->size ? simulation->size : x2;
        y2 = y2 > simulation->size ? simulation->size : y2;


        // dodaje tylko w pierwszym rzędzie
        for (int j = y1; j <= y2; j++) {
            for (int i = x1; i <= x2; i++) {
               // e_simulation->AddWall(i, j);
            }
        }
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent* e)
{
    QPoint mousePosition = e->pos();

    if (e->buttons() == 2) {
        int xPress = lastPosition.x();
        int yPress = lastPosition.y();
        int xCurrent = mousePosition.x();
        int yCurrent = mousePosition.y();

        // dane do wektora zmiany położenia myszki
        float dx = xCurrent - xPress;
        float dy = yCurrent - yPress;

        //float lenght = sqrt(dx * dx + dy * dy);
        //float xNormalized = (float)dx * SPEED_SCALE / lenght;
        //float yNormalized = (float)dy * SPEED_SCALE / lenght;

        simulation->AddVelocity(xCurrent, yCurrent, SPEED_CHANGE_RADIUS, (float)dy * SPEED_SCALE, (float)dx * SPEED_SCALE);

    }
    lastPosition = e->pos();
}


void MyGraphicsView::start() {
    QString tmp = it->toPlainText();
    QByteArray bytearray = tmp.toLocal8Bit();
    const char* string = bytearray.data();
    this->interval = atoi(string);

    //Its must be here and not in constructor because this->width() and this->height() in constractor return wrong size
    pixels.reset((float*)malloc(sizeof(float) * (this->width() + 2) * (this->width() + 2)));
    image.reset(new QImage(this->width(), this->height(), QImage::Format_RGB888));
    pixmap = QPixmap(this->width(), this->height());
    scene.reset(new QGraphicsScene(this));
    pixMapItem = scene->addPixmap(pixmap);
    //pixMapItemTEST.reset((scene->addPixmap(pixmap)));
    this->setScene(&*scene);
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    float diff = ((float)d->value()) / 100000;
    float visc = ((float)v->value()) / 100000;

    switch (kind->currentIndex()) {
        //prawie orginał
    case 0:
        factory.reset(new FactoryNotEditedSimulation());
        simulation.reset(factory->CreateSimulation(this->width(), diff, visc, ((float)this->interval) / 1000));
        break;
        //w sumie to teraz bardziej przypomina orginał niż orginalny
    case 1:
        //dynamic cast
        factory.reset(new FactoryEditedSimulation());

        // tu jest błąd
        simulation.reset(factory->CreateSimulation(this->width(), diff, visc, ((float)this->interval) / 1000));

        
        e_simulation = std::dynamic_pointer_cast<EditedSimulation>(simulation);

        break;
        //openMP
    case 2:
        break;
        //cuda
    case 3:
        break;
    case 4:
        factory.reset(new FactoryMA());
        simulation.reset(factory->CreateSimulation(this->width(), diff, visc, ((float)this->interval) / 1000));
        break;
    }

    tmp = ft->toPlainText();
    bytearray = tmp.toLocal8Bit();
    string = bytearray.data();
    this->frameTime = atoi(string);

    timer->setInterval(this->frameTime);
    timer->start();
}

void MyGraphicsView::refresh(){
    simulation->NextFrame(pixels);
    for (int i = 1; i < this->width() ; i++) {
        for (int j = 1; j < this->height() ; j++) {
            image->setPixelColor(i - 1, j - 1, getColor(pixels[IX(i, j)]));
        }
    }
    pixMapItem->setPixmap(QPixmap::fromImage(*image));
    this->show();
    //qDebug() << combined_density;
}

QColor MyGraphicsView::getColor(float x) {
    if (x < 0) {
        return QColor(0, 255, 0, 120);
    }
    else {
        int y = (int)(x * 255);
        return QColor(y, y, y, 255);
    }
}