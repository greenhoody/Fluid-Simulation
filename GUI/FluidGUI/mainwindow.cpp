#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Simulation/Simulator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->giveRequiredElements(ui->ViscositySlider,ui->DiffusionSlider,ui->plainTextEdit);

}

MainWindow::~MainWindow()
{
    delete ui;
}
