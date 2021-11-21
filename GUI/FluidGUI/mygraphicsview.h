#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include "../Simulation/Simulation.h"


class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent);
    QTimer *timer;
    Simulation *simulation;
    

public slots:
    void refresh();
    void start();
    void intervalUpdate(int i);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    int interval;
};

#endif // MYGRAPHICSVIEW_H
