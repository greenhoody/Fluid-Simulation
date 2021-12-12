#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "qslider.h"
#include "../Simulation/Simulation.h"
#include "qplaintextedit.h"


class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent);
    void giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit* ts);

    QTimer *timer;
    Simulation *simulation = nullptr;
    QSlider *v, *d;
    QPlainTextEdit* ts;
    QImage* image;
    QPixmap pixmap;
    QGraphicsPixmapItem* pixMapItem;
    float* pixels;

public slots:
    void refresh();
    void start();
    //void intervalUpdate(int i);

protected:
   // MyGraphicsView(QWidget* parent);
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QColor getColor(float x);
    int interval;

    QGraphicsScene* scene;
};

#endif // MYGRAPHICSVIEW_H
