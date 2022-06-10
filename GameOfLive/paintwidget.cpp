#include "paintwidget.h"
#include "ui_paintwidget.h"
#include "QPainter"
#include "QMouseEvent"
#include "QThread"

bool ifConfirmed = false, ifLeftButtonPressed = false, ifRightButtonPressed = false;
const QBrush cursorPointBrush = QBrush(Qt::NoBrush);                    //Константа для кисти курсора
short size_x, size_y, fSize_x, fSize_y;     //Размеры поля в пикселях и условных клетках
const int maxX = 3000, maxY = 1500;     //Константы максимального размера поля в пикселях
QColor points[maxX/10][maxY/10];     //Создание массива точек исходя из максимальных размеров поля
short speed = 1000;

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintWidget)
{
    connect( &timer, SIGNAL( timeout() ), SLOT( gameProcess() ) );
    ui->setupUi(this);
    this->setMouseTracking(true);   //Включение отслеживания мыши
    QWidget::showFullScreen();      //Запуск в полноэкранном режиме

    if (geometry().width()/10*10 - 270 > maxX){     //Установка размеров поля
        size_x = maxX;
        fSize_x = size_x/10;
    }
    else
    {
        size_x = geometry().width()/10*10 - 270;
        fSize_x = size_x/10;
    }
    if (geometry().height()/10*10 - 40 > maxY)
    {
        size_y = maxY;
        fSize_y = size_y/10;
    }
    else
    {
        size_y = geometry().height()/10*10 - 40;
        fSize_y = size_y/10;
    }

    for (short i = 0; i < maxX/10; i++)           //Зануление цвета точек
        for (short j = 0; j < maxY/10; j++)
            points[i][j] = QColor(0, 0, 0, 0);
}

PaintWidget::~PaintWidget()
{
    delete ui;
}

QColor newPointColor(int x, int y){             //Установка нового цвета для точки в зависимости от соседей
    QColor color;
    color.setRgb(0, 0, 0, 0);

    if (points[x-1][y] != QColor(0, 0, 0, 0))
        if(x-1 >= 0)
        {
            if (color == QColor(0, 0, 0, 0))
                color = points[x-1][y];
            else
                color = QColor((color.red() + points[x-1][y].red())/2, (points[x-1][y].green() + color.green())/2, (points[x-1][y].blue() + color.blue())/2);
        }
        else
            if (color == QColor(0, 0, 0, 0))
                color = points[fSize_x-1][y];
            else
                color = QColor((color.red() + points[fSize_x-1][y].red())/2, (points[fSize_x-1][y].green() + color.green())/2, (points[fSize_x-1][y].blue() + color.blue())/2);

    if (points[x][y-1] != QColor(0, 0, 0, 0))
        if(y-1 >= 0)
        {
            if (color == QColor(0, 0, 0, 0))
                color = points[x][y-1];
            else
                color = QColor((color.red() + points[x][y-1].red())/2, (points[x][y-1].green() + color.green())/2, (points[x][y-1].blue() + color.blue())/2);
        }
    else
        if (color == QColor(0, 0, 0, 0))
            color = points[x][fSize_y-1];
        else
            color = QColor((color.red() + points[x][fSize_y-1].red())/2, (points[x][fSize_y-1].green() + color.green())/2, (points[x][fSize_y-1].blue() + color.blue())/2);

    if (points[x+1][y] != QColor(0, 0, 0, 0))
        if(x+1 < fSize_x)
        {
            if (color == QColor(0, 0, 0, 0))
                color = points[x+1][y];
            else
                color = QColor((color.red() + points[x+1][y].red())/2, (points[x+1][y].green() + color.green())/2, (points[x+1][y].blue() + color.blue())/2);
        }
        else
            if (color == QColor(0, 0, 0, 0))
                color = points[0][y];
            else
                color = QColor((color.red() + points[0][y].red())/2, (points[0][y].green() + color.green())/2, (points[0][y].blue() + color.blue())/2);

    if (points[x][y+1] != QColor(0, 0, 0, 0))
        if(y+1 < fSize_y)
        {
            if (color == QColor(0, 0, 0, 0))
                color = points[x][y+1];
            else
                color = QColor((color.red() + points[x][y+1].red())/2, (points[x][y+1].green() + color.green())/2, (points[x][y+1].blue() + color.blue())/2);
        }
        else
            if (color == QColor(0, 0, 0, 0))
                color = points[x][0];
            else
                color = QColor((color.red() + points[x][0].red())/2, (points[x][0].green() + color.green())/2, (points[x][0].blue() + color.blue())/2);
    if(x-1 >= 0)
    {
        if(y-1 >= 0)
        {
            if (points[x-1][y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x-1][y-1];
                else
                    color = QColor((color.red() + points[x-1][y-1].red())/2, (points[x-1][y-1].green() + color.green())/2, (points[x-1][y-1].blue() + color.blue())/2);
        }
        else
            if (points[x-1][fSize_y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x-1][fSize_y-1];
                else
                    color = QColor((color.red() + points[x-1][fSize_y-1].red())/2, (points[x-1][fSize_y-1].green() + color.green())/2, (points[x-1][fSize_y-1].blue() + color.blue())/2);
    }
    else
        if(y-1 >= 0)
        {
            if (points[fSize_x-1][y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[fSize_x-1][y-1];
                else
                    color = QColor((color.red() + points[fSize_x-1][y-1].red())/2, (points[fSize_x-1][y-1].green() + color.green())/2, (points[fSize_x-1][y-1].blue() + color.blue())/2);
        }
        else
            if (points[fSize_x-1][fSize_y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[fSize_x-1][fSize_y-1];
                else
                    color = QColor((color.red() + points[fSize_x-1][fSize_y-1].red())/2, (points[fSize_x-1][fSize_y-1].green() + color.green())/2, (points[fSize_x-1][fSize_y-1].blue() + color.blue())/2);
    if(x-1 >= 0)
    {
        if(y+1 < fSize_y)
        {
            if (points[x-1][y+1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x-1][y+1];
                else
                    color = QColor((color.red() + points[x-1][y+1].red())/2, (points[x-1][y+1].green() + color.green())/2, (points[x-1][y+1].blue() + color.blue())/2);
        }
        else
            if (points[x-1][0] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x-1][0];
                else
                    color = QColor((color.red() + points[x-1][0].red())/2, (points[x-1][0].green() + color.green())/2, (points[x-1][0].blue() + color.blue())/2);
    }
    else
        if(y+1 < fSize_y)
        {
            if (points[fSize_x-1][y+1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[fSize_x-1][y+1];
                else
                    color = QColor((color.red() + points[fSize_x-1][y+1].red())/2, (points[fSize_x-1][y+1].green() + color.green())/2, (points[fSize_x-1][y+1].blue() + color.blue())/2);
        }
        else
            if (points[fSize_x-1][0] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[fSize_x-1][0];
                else
                    color = QColor((color.red() + points[fSize_x-1][0].red())/2, (points[fSize_x-1][0].green() + color.green())/2, (points[fSize_x-1][0].blue() + color.blue())/2);
    if(x+1 < fSize_x)
    {
        if (y-1 >= 0)
        {
            if (points[x+1][y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x+1][y-1];
                else
                    color = QColor((color.red() + points[x+1][y-1].red())/2, (points[x+1][y-1].green() + color.green())/2, (points[x+1][y-1].blue() + color.blue())/2);
        }
        else
            if (points[x+1][fSize_y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x+1][fSize_y-1];
                else
                    color = QColor((color.red() + points[x+1][fSize_y-1].red())/2, (points[x+1][fSize_y-1].green() + color.green())/2, (points[x+1][fSize_y-1].blue() + color.blue())/2);
    }
    else
        if (y-1 >= 0)
        {
            if (points[0][y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[0][y-1];
                else
                    color = QColor((color.red() + points[0][y-1].red())/2, (points[0][y-1].green() + color.green())/2, (points[0][y-1].blue() + color.blue())/2);
        }
        else
            if (points[0][fSize_y-1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[0][fSize_y-1];
                else
                    color = QColor((color.red() + points[0][fSize_y-1].red())/2, (points[0][fSize_y-1].green() + color.green())/2, (points[0][fSize_y-1].blue() + color.blue())/2);
    if(x+1 < fSize_x)
    {
        if(y+1 < fSize_y)
        {
            if (points[x+1][y+1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x+1][y+1];
                else
                    color = QColor((color.red() + points[x+1][y+1].red())/2, (points[x+1][y+1].green() + color.green())/2, (points[x+1][y+1].blue() + color.blue())/2);
        }
        else
            if (points[x+1][0] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[x+1][0];
                else
                    color = QColor((color.red() + points[x+1][0].red())/2, (points[x+1][0].green() + color.green())/2, (points[x+1][0].blue() + color.blue())/2);
    }
    else
        if(y+1 < fSize_y)
        {
            if (points[0][y+1] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[0][y+1];
                else
                    color = QColor((color.red() + points[0][y+1].red())/2, (points[0][y+1].green() + color.green())/2, (points[0][y+1].blue() + color.blue())/2);
        }
        else
            if (points[0][0] != QColor(0, 0, 0, 0))
                if (color == QColor(0, 0, 0, 0))
                    color = points[0][0];
                else
                    color = QColor((color.red() + points[0][0].red())/2, (points[0][0].green() + color.green())/2, (points[0][0].blue() + color.blue())/2);

    return (color);
}

QColor setColor(int y){                 //Установка начального цвета (перед запуском)
    QColor color;

    if ((y < fSize_y/6) | (y > fSize_y/6*5))
        color.setRgb(0, 0, 255, 255);
    else if ((y < fSize_y/3) | (y > fSize_y/3*2))
        color.setRgb(0, 255, 0, 255);
    else
        color.setRgb(255, 0, 0, 255);

    return (color);
}

void drawFeild(QPainter &painter){      //Отрисовка поля

    painter.setPen(QPen(Qt::black, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(19, 19, size_x+1, size_y+1);
}

void drawPoint(QPainter &painter, QBrush pointBrush, short x, short y)      //Отрисовка точки
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(pointBrush);
    painter.drawRect(x*10+20, y*10+20, 10, 10);
}

void PaintWidget::paintEvent(QPaintEvent *event){       //Событие, вызываемое repaint();
    QPainter painter(this);

    drawFeild(painter);
    painter.setPen(Qt::NoPen);
    for (short i = 0; i < fSize_x; i++)                   //Цикл отрисовки точек
        for (short j = 0; j < fSize_y; j++){
            if (points[i][j] != QColor(0, 0, 0, 0))
                drawPoint(painter, QBrush(points[i][j], Qt::SolidPattern), i, j);
        }
    if (!ifConfirmed){                                       //Отрисовка точки у курсора, пока не нажата Confirm
        if ((QCursor::pos().x() < size_x+20) & (QCursor::pos().y() < size_y+20) & (QCursor::pos().x() > 20) & (QCursor::pos().y() > 20)){
            painter.setBrush(cursorPointBrush);
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            int posX = QCursor::pos().x() / 10 * 10;
            int posY = QCursor::pos().y() / 10 * 10;
            painter.drawRect(posX, posY, 10, 10);
        }
    }
}

void PaintWidget::gameProcess()
{
    QColor newPoints[maxX/10][maxY/10];               //Создание массива новых точек
    for (short i = 0; i < fSize_x; i++)               //Зануление цвета точек нового поля
        for (short j = 0; j < fSize_y; j++)
            newPoints[i][j] = QColor(0, 0, 0, 0);
    short n_count = 0;
    for(short i=0; i < fSize_x; i++){             //Проверка выживания и появления клетки
        for(short j=0; j < fSize_y; j++){
            if(i-1 >= 0)
            {
                if(points[i-1][j] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(points[fSize_x-1][j] != QColor(0, 0, 0, 0)) n_count++;
            if(j-1 >= 0)
            {
                if(points[i][j-1] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(points[i][fSize_y-1] != QColor(0, 0, 0, 0)) n_count++;
            if(i+1 < fSize_x)
            {
                if(points[i+1][j] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(points[0][j] != QColor(0, 0, 0, 0)) n_count++;
            if(j+1 < fSize_y)
            {
                if(points[i][j+1] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(points[i][0] != QColor(0, 0, 0, 0)) n_count++;
            if(i-1 >= 0)
            {
                if(j-1 >= 0)
                {
                    if(points[i-1][j-1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[i-1][fSize_y-1] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(j-1 >= 0)
                {
                    if(points[fSize_x-1][j-1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[fSize_x-1][fSize_y-1] != QColor(0, 0, 0, 0)) n_count++;
            if(i-1 >= 0)
            {
                if(j+1 < fSize_y)
                {
                    if(points[i-1][j+1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[i-1][0] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(j+1 < fSize_y)
                {
                    if(points[fSize_x-1][j+1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[fSize_x-1][0] != QColor(0, 0, 0, 0)) n_count++;
            if(i+1 < fSize_x)
            {
                if (j-1 >= 0)
                {
                    if(points[i+1][j-1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[i+1][fSize_y-1] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if (j-1 >= 0)
                {
                    if(points[0][j-1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[0][fSize_y-1] != QColor(0, 0, 0, 0)) n_count++;
            if(i+1 < fSize_x)
            {
                if(j+1 < fSize_y)
                {
                    if(points[i+1][j+1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[i+1][0] != QColor(0, 0, 0, 0)) n_count++;
            }
            else
                if(j+1 < fSize_y)
                {
                    if(points[0][j+1] != QColor(0, 0, 0, 0)) n_count++;
                }
                else
                    if(points[0][0] != QColor(0, 0, 0, 0)) n_count++;
            if(points[i][j] != QColor(0, 0, 0, 0))
            {
                if(n_count < 2) newPoints[i][j] = QColor(0, 0, 0, 0);
                else if(n_count > 3) newPoints[i][j] = QColor(0, 0, 0, 0);
                else if((n_count == 2) | (n_count == 3)) newPoints[i][j] = newPointColor(i, j);
            }
            else if(n_count == 3)
                newPoints[i][j] = newPointColor(i, j);
            n_count = 0;
        }
    }
    for (short i = 0; i < fSize_x; i++)             //Запись нового поля
        for (short j = 0; j < fSize_y; j++)
            points[i][j] = newPoints[i][j];
    repaint();
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event){       //Событие, срабатывающее при движении мыши
    if (!ifConfirmed){                  //Отрисовка или удаление точки, если зажата соответствующая клавиша мыши
        if (ifLeftButtonPressed)
            if ((QCursor::pos().x() < size_x+20) & (QCursor::pos().y() < size_y+20) & (QCursor::pos().x() > 20) & (QCursor::pos().y() > 20))
                points[QCursor::pos().x()/10-2][QCursor::pos().y()/10-2] = QColor(setColor(QCursor::pos().y()/10-2));
        if (ifRightButtonPressed)
            if ((QCursor::pos().x() < size_x+20) & (QCursor::pos().y() < size_y+20) & (QCursor::pos().x() > 20) & (QCursor::pos().y() > 20))
                points[QCursor::pos().x()/10-2][QCursor::pos().y()/10-2] = QColor(0, 0, 0, 0);
        repaint();
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event){      //Событие, срабатывающее при нажатии любой клавиши мыши
    if (!ifConfirmed)                   //Отрисовка или удаление точки, если нажата соответствующая клавиша мыши
    {
        if (event->button() == Qt::LeftButton)
        {
            ifLeftButtonPressed = true;         //Переменная указывает на то, что ЛКМ сейчас нажата
            if ((QCursor::pos().x() < size_x+20) & (QCursor::pos().y() < size_y+20) & (QCursor::pos().x() > 20) & (QCursor::pos().y() > 20))
            {
                points[QCursor::pos().x()/10-2][QCursor::pos().y()/10-2] = QColor(setColor(QCursor::pos().y()/10-2));
                repaint();
            }
        }
        if (event->button() == Qt::RightButton)
        {
            ifRightButtonPressed = true;         //Переменная указывает на то, что ПКМ сейчас нажата
            if ((QCursor::pos().x() < size_x+20) & (QCursor::pos().y() < size_y+20) & (QCursor::pos().x() > 20) & (QCursor::pos().y() > 20))
            {
                points[QCursor::pos().x()/10-2][QCursor::pos().y()/10-2] = QColor(0, 0, 0, 0);
                repaint();
            }
        }
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)        //Событие, срабатывающее при поднятии любой клавиши мыши
{
    if (!ifConfirmed)
    {
        if (event->button() == Qt::LeftButton)
            ifLeftButtonPressed = false;            //Переменная указывает на то, что ЛКМ сейчас не нажата
        if (event->button() == Qt::RightButton)
            ifRightButtonPressed = false;           //Переменная указывает на то, что ПКМ сейчас не нажата
    }
}

void PaintWidget::on_confirmButton_clicked()                    //Событие, срабатывающее при нажатии кнопки Confirm
{
    ifConfirmed = true;
    this->setMouseTracking(false);          //Движение мыши больше не отслеживается
    repaint();
    timer.start( speed );
    ui->confirmButton->setEnabled(false);
    ui->randomButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
}

void PaintWidget::on_exitButton_clicked()                       //Событие, срабатывающее при нажатии кнопки Exit
{
    timer.stop();
    QThread::sleep(1);
    exit(0);
}

void PaintWidget::on_randomButton_clicked()                 //Событие, срабатывающее при нажитии кнопки Random generation
{
    bool tRandom;
    for (int i=0; i<fSize_x; i++)
        for (int j=0; j<fSize_y; j++)
        {
            tRandom = rand() % 2;
            if (tRandom)
                points[i][j] = setColor(j);
        }
}

void PaintWidget::on_horizontalSlider_valueChanged(int value)           //Событие, срабатывающее при изменении положения ползунка скорости
{
    speed = 1000/value;
    if (timer.isActive())
    {
        timer.stop();
        timer.start( speed );
    }
}

void PaintWidget::on_pauseButton_clicked()              //Событие, срабатывающее при нажитии кнопки Play/Pause
{
    if (timer.isActive())
        timer.stop();
    else
        timer.start( speed );
}

void PaintWidget::on_clearButton_clicked()          //Событие, срабатывающее при нажитии кнопки Restart
{
    if (timer.isActive())
        timer.stop();
    ifConfirmed = false;
    ui->randomButton->setEnabled(true);
    ui->confirmButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    for (short i = 0; i < maxX/10; i++)           //Зануление цвета точек
        for (short j = 0; j < maxY/10; j++)
            points[i][j] = QColor(0, 0, 0, 0);
}
