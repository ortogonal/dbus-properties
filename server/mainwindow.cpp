#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "example.h"
#include "appliance.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Example *example = new Example(this);
    connect(example, &Example::tickChanged, this, [example, this]() {
        setWindowTitle(QString::number(example->tick()));
    });

    connect(ui->red, &QSlider::valueChanged, this, [this, example]() {
        example->setRed(ui->red->value());
    });
    connect(ui->green, &QSlider::valueChanged, this, [this, example]() {
        example->setGreen(ui->green->value());
    });
    connect(ui->blue, &QSlider::valueChanged, this, [this, example]() {
        example->setBlue(ui->blue->value());
    });

    Appliance *appliance = new Appliance();
    connect(ui->running, &QCheckBox::stateChanged, this, [this, appliance]() {
        appliance->setRunning(ui->running->isChecked());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
