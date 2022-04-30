#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <vector>
#include <QComboBox>
#include <memory>

#include "qslider.h"
#include "qplaintextedit.h"

#include "factory.h"
#include "simulation.h"


class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public slots:
    void refresh();
    void start();
    //void intervalUpdate(int i);

public:
    MyGraphicsView(QWidget *parent);
    void giveRequiredElements(QSlider* v, QSlider* d, QPlainTextEdit* it, QPlainTextEdit* ft, QComboBox* kind);

    QComboBox * kind;
    QSlider * v, * d;
    QPlainTextEdit * it, * ft;
    QTimer* timer;

    std::unique_ptr<Factory> factory;
    Simulation* simulation = nullptr;
    std::unique_ptr<QImage> image;
    float* pixels;

    //std::unique_ptr<QGraphicsPixmapItem> pixMapItemTEST;
    QGraphicsPixmapItem* pixMapItem = nullptr;

    QPixmap pixmap;

protected:
   // MyGraphicsView(QWidget* parent);
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    QColor getColor(float x);
    int interval, frameTime;
    QPoint pressPosition;
    QPoint lastPosition;

    int pressedKey;


    std::unique_ptr<QGraphicsScene> scene;
};

#endif // MYGRAPHICSVIEW_H
