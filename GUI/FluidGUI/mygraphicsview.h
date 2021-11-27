#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
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
    Simulation *simulation;
    QSlider *v, *d;
    QPlainTextEdit* ts;
    

public slots:
    void refresh();
    void start();
    //void intervalUpdate(int i);

protected:
   // MyGraphicsView(QWidget* parent);
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    int interval;
};

#endif // MYGRAPHICSVIEW_H
