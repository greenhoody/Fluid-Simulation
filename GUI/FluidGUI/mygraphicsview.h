#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "qslider.h"
#include "../Simulation/Simulation.h"
#include "qplaintextedit.h"
#include "../Simulation2/Simulation2.h"
#include <vector>


class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public slots:
    void refresh();
    void start();
    //void intervalUpdate(int i);

public:
    MyGraphicsView(QWidget *parent);
    void giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit* ts);

    QTimer *timer;
    Simulation *simulation = nullptr;
    Simulation2* simulation2 = nullptr;
    QSlider *v, *d;
    QPlainTextEdit* ts;
    QImage* image;
    QPixmap pixmap;
    QGraphicsPixmapItem* pixMapItem;
    float* pixels;



protected:
   // MyGraphicsView(QWidget* parent);
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    QColor getColor(float x);
    int interval;
    QPoint pressPosition;
    QPoint lastPosition;

    QGraphicsScene* scene;
};

#endif // MYGRAPHICSVIEW_H
