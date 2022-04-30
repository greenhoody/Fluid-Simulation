#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Simulation/Simulator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //it may be only way to pass required components from mainview to child
    ui->graphicsView->giveRequiredElements(ui->ViscositySlider,ui->DiffusionSlider,ui->iterationTime, ui->frameTime, ui->comboBox);

}

MainWindow::~MainWindow()
{
    delete ui;
}
