#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class PaintWidget;
}

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);
    ~PaintWidget();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_confirmButton_clicked();
    void on_exitButton_clicked();
    void gameProcess();
    void on_randomButton_clicked();
    void on_horizontalSlider_valueChanged(int value);

    void on_pauseButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::PaintWidget *ui;
    QTimer timer;
};

#endif // PAINTWIDGET_H
