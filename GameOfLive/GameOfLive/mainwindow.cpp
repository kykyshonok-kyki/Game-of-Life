#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void drawPoint(QPainter &paint, QPen pen, short x, short y)
{
    paint.setPen(QPen(Qt::black));
    QLine(x, y, x+10, y);
    QLine(x, y, x, y+10);
    QLine(x+10, y, x+10, y+10);
    QLine(x, y+10, x+10, y+10);

    paint.setPen(pen);
    QRectF(x+1, y+1, 8, 8);
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter paint(this);
    QPen pen;
    pen.setColor(Qt::red);

    drawPoint(paint, pen, 10, 10);
}

void MainWindow::on_playButton_clicked()
{
    QPainter paint(this);
    QPen pen;
    pen.setColor(Qt::red);

    drawPoint(paint, pen, 30, 30);
}
