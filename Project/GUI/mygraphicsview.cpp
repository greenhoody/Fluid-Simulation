#include "mygraphicsview.h"

#include "cudafactory.h"
#include "openaccfactory.h"
#include "noteditedfactory.h"
#include "factory.h"

#include <QMouseEvent>
#include <QDebug>

#include <chrono>
#include <inttypes.h>

#define IX(i,j) ((i)+(simulation->size+2)*(j))

constexpr auto SPEED_SCALE = 2.0f;
constexpr auto SPEED_CHANGE_RADIUS = 3;

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MyGraphicsView::refresh);
}

void MyGraphicsView::giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit* it, QPlainTextEdit* ft, QComboBox* kind)
{
    // it's must be here because for now its best thing to get values from thes widgets in code
    this->v = v;
    this->d = d;
    this->it = it;
    this->ft = ft;
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
            simulation->AddDensity(x1,x2,y1,y2,0.7f);
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

void MyGraphicsView::refresh(){  
    using namespace std::chrono;
    uint64_t time1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    simulation->NextFrame(pixels);

    uint64_t time2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
      uint64_t t = time2 - time1;
      printf("%" PRIu64 "\n", t);

    for (int i = 1; i < this->width() ; i++) {
        for (int j = 1; j < this->height() ; j++) {
            image->setPixelColor(i - 1, j - 1, getColor(pixels[IX(i, j)]));
        }
    }
    pixMapItem->setPixmap(QPixmap::fromImage(*image));
    this->show();
}

//

 //
/*  */

QColor MyGraphicsView::getColor(float x) {
    if (x < 0) {
        return QColor(0, 255, 0, 120);
    }
    else {
        int y = (int)(x * 255);
        return QColor(y, y, y, 255);
    }
}

void MyGraphicsView::start() {
    QString tmp = it->toPlainText();
    QByteArray bytearray = tmp.toLocal8Bit();
    char* string = bytearray.data();
    this->interval = atoi(string);

    //Its must be here and not in constructor because this->width() and this->height() in constractor return wrong size
    pixels = (float*)malloc(sizeof(float) * (this->width() + 2) * (this->width() + 2));
    image.reset(new QImage(this->width(), this->height(), QImage::Format_RGB888));

    pixmap = QPixmap(this->width(), this->height());
    scene.reset(new QGraphicsScene(this));

    //if (pixMapItem != nullptr)
    //{
    //    free(pixMapItem);
    //}

    pixMapItem = scene->addPixmap(pixmap);

    //pixMapItemTEST.reset((scene->addPixmap(pixmap)));

    this->setScene(&*scene);
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    float diff = ((float)d->value()) / 100000;
    float visc = ((float)v->value()) / 100000;

    switch (kind->currentIndex()) {
        //prawie orginał
    case 0:
        if (simulation != nullptr)
        {
            simulation->~Simulation();
            free(simulation);
        }
        factory.reset(new NotEditedFactory());
        simulation = factory->CreateSimulation(this->width(), diff, visc, ((float)this->interval) / 1000);
        break;

        //openACC
    case 1:
        if (simulation != nullptr)
        {
            simulation->~Simulation();
            free(simulation);
        }
        factory.reset(new OpenACCFactory());
        simulation = factory->CreateSimulation(this->width(), diff, visc, ((float)this->interval) / 1000);
        break;
        //cuda
    case 2:
        factory.reset(new CudaFactory());

        if (simulation != nullptr)
        {
            simulation->~Simulation();
            free(simulation);
        }

        simulation = factory->CreateSimulation(this->width(), diff, visc, ((float)this->interval) / 1000);

        break;

    }

    tmp = ft->toPlainText();
    bytearray = tmp.toLocal8Bit();
    string = bytearray.data();
    this->frameTime = atoi(string);

    timer->setInterval(this->frameTime);
    timer->start();
}
