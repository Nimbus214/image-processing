// mainwindow.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QWheelEvent>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSlider>
#include <QLabel>
#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>
#include <QtConcurrent>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Инициализация UI и установка общих стилей
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(116, 116, 116);");
    this->centralWidget()->setStyleSheet("background-color: rgb(116, 116, 116);");
    ui->graphicsView->setStyleSheet("background-color: rgb(73, 73, 73);");
    showMaximized();

    // Настройка макета
    QHBoxLayout *layout = new QHBoxLayout(ui->centralwidget);
    layout->addWidget(ui->graphicsView);
    ui->centralwidget->setLayout(layout);

    // Создание и настройка панели инструментов
    QToolBar *toolbar = new QToolBar("My Toolbar", this);
    toolbar->setStyleSheet("color: white; background-color: rgb(100, 100, 100);");

    // Добавление кнопки "Рука" на панель инструментов
    QAction *handAction = new QAction("Рука", this);
    handAction->setIcon(QIcon(":/images/hand.png"));
    connect(handAction, &QAction::triggered, this, &MainWindow::enableHandTool);
    toolbar->addAction(handAction);

    // Добавление панели инструментов на левую сторону главного окна
    addToolBar(Qt::LeftToolBarArea, toolbar);

    // Создание меню
    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    QMenuBar *menuBar = this->menuBar();
    menuBar->setStyleSheet("color: white; background-color: rgb(73, 73, 73);");
    fileMenu->setStyleSheet("color: white; background-color: rgb(73, 73, 73);");

    // Добавление действий в меню
    QAction *openAction = fileMenu->addAction(tr("&Открыть"));
    QAction *saveAction = fileMenu->addAction(tr("&Сохранить"));
    QAction *exitAction = fileMenu->addAction(tr("&Выйти"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QCoreApplication::quit);

    // Связывание действий с соответствующими слотами
    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    // Настройка QGraphicsView
    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setMaximumSize(950, 950);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Создание панели инструментов для настроек
    QToolBar *adjustmentsToolbar = new QToolBar("Adjustments", this);
    adjustmentsToolbar->setStyleSheet("color: white; background-color: rgb(100, 100, 100);");
    addToolBar(Qt::RightToolBarArea, adjustmentsToolbar);

    // Настройки яркости
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setMinimum(-150);
    brightnessSlider->setMaximum(150);
    brightnessSlider->setFixedWidth(150);
    connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::adjustBrightness);

    QLineEdit *brightnessLineEdit = new QLineEdit(this);
    brightnessLineEdit->setValidator(new QIntValidator(-150, 150, this));
    brightnessLineEdit->setFixedWidth(30);

    QWidget *brightnessWidget = new QWidget(this);
    QHBoxLayout *brightnessLayout = new QHBoxLayout(brightnessWidget);
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessLineEdit);
    brightnessLayout->setContentsMargins(0, 0, 0, 0);
    connect(brightnessSlider, &QSlider::valueChanged, this, [brightnessLineEdit](int value){
        brightnessLineEdit->setText(QString::number(value));
    });

    adjustmentsToolbar->addWidget(new QLabel("Яркость:"));
    adjustmentsToolbar->addWidget(brightnessWidget);

    // Настройки цветового тона
    QSlider *hueSlider = new QSlider(Qt::Horizontal);
    hueSlider->setMinimum(-180);
    hueSlider->setMaximum(180);
    hueSlider->setFixedWidth(150);
    connect(hueSlider, &QSlider::valueChanged, this, &MainWindow::adjustHue);

    QLineEdit *hueLineEdit = new QLineEdit(this);
    hueLineEdit->setValidator(new QIntValidator(-180, 180, this));
    hueLineEdit->setFixedWidth(30);

    QWidget *hueWidget = new QWidget(this);
    QHBoxLayout *hueLayout = new QHBoxLayout(hueWidget);
    hueLayout->addWidget(hueSlider);
    hueLayout->addWidget(hueLineEdit);
    hueLayout->setContentsMargins(0, 0, 0, 0);
    connect(hueSlider, &QSlider::valueChanged, this, [hueLineEdit](int value){
        hueLineEdit->setText(QString::number(value));
    });

    adjustmentsToolbar->addWidget(new QLabel("Цветовой тон:"));
    adjustmentsToolbar->addWidget(hueWidget);

    // Настройки насыщенности
    QSlider *saturationSlider = new QSlider(Qt::Horizontal);
    saturationSlider->setMinimum(-250);
    saturationSlider->setMaximum(250);
    saturationSlider->setFixedWidth(150);
    connect(saturationSlider, &QSlider::valueChanged, this, &MainWindow::adjustSaturation);

    QLineEdit *saturationLineEdit = new QLineEdit(this);
    saturationLineEdit->setValidator(new QIntValidator(-250, 250, this));
    saturationLineEdit->setFixedWidth(30);

    QWidget *saturationWidget = new QWidget(this);
    QHBoxLayout *saturationLayout = new QHBoxLayout(saturationWidget);
    saturationLayout->addWidget(saturationSlider);
    saturationLayout->addWidget(saturationLineEdit);
    saturationLayout->setContentsMargins(0, 0, 0, 0);
    connect(saturationSlider, &QSlider::valueChanged, this, [saturationLineEdit](int value){
        saturationLineEdit->setText(QString::number(value));
    });

    adjustmentsToolbar->addWidget(new QLabel("Насыщенность:"));
    adjustmentsToolbar->addWidget(saturationWidget);

    // Связывание изменений в QLineEdit с обновлением слайдеров
    connect(brightnessLineEdit, &QLineEdit::textChanged, this, [brightnessSlider](const QString &text){
        brightnessSlider->setValue(text.toInt());
    });

    connect(hueLineEdit, &QLineEdit::textChanged, this, [hueSlider](const QString &text){
        hueSlider->setValue(text.toInt());
    });

    connect(saturationLineEdit, &QLineEdit::textChanged, this, [saturationSlider](const QString &text){
        saturationSlider->setValue(text.toInt());
    });

    // Кнопки для поворота изображения
    QPushButton *rotateClockwiseButton = new QPushButton(this);
    QPushButton *rotateCounterClockwiseButton = new QPushButton(this);

    rotateClockwiseButton->setIcon(QIcon(":/images/po.png"));
    rotateClockwiseButton->setIconSize(QSize(32, 32));
    rotateClockwiseButton->setToolTip("Поворот по часовой");

    rotateCounterClockwiseButton->setIcon(QIcon(":/images/protiv.png"));
    rotateCounterClockwiseButton->setIconSize(QSize(32, 32));
    rotateCounterClockwiseButton->setToolTip("Поворот против часовой");

    connect(rotateClockwiseButton, &QPushButton::clicked, this, &MainWindow::rotateImageClockwise);
    connect(rotateCounterClockwiseButton, &QPushButton::clicked, this, &MainWindow::rotateImageCounterClockwise);

    // Контейнер для кнопок поворота
    QWidget *rotateButtonsWidget = new QWidget(this);
    QHBoxLayout *rotateButtonsLayout = new QHBoxLayout(rotateButtonsWidget);
    rotateButtonsLayout->addWidget(rotateCounterClockwiseButton);
    rotateButtonsLayout->addWidget(rotateClockwiseButton);

    // Добавление контейнера с кнопками поворота на панель инструментов
    adjustmentsToolbar->addWidget(rotateButtonsWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::rotateImageClockwise()
{
    if (!originalPixmap.isNull()) {
        originalPixmap = originalPixmap.transformed(QTransform().rotate(90));
        updateImage();
    }
}

void MainWindow::rotateImageCounterClockwise()
{
    if (!originalPixmap.isNull()) {
        originalPixmap = originalPixmap.transformed(QTransform().rotate(-90));
        updateImage();
    }
}

void MainWindow::updateImage()
{
    if (pixmapItem == nullptr) {
        return;
    }

    QImage originalImage = originalPixmap.toImage();
    QImage adjustedImage(originalImage.size(), QImage::Format_ARGB32);

    // Разбиваем изображение на несколько частей с перекрытием
    int numThreads = QThread::idealThreadCount();
    int chunkHeight = originalImage.height() / numThreads;
    int overlap = 1; // Количество пикселей перекрытия между частями
    QList<QRect> rects;
    for (int i = 0; i < numThreads; ++i) {
        int startY = i * chunkHeight - overlap;
        int endY = (i == numThreads - 1) ? originalImage.height() : ((i + 1) * chunkHeight + overlap);
        startY = qBound(0, startY, originalImage.height());
        endY = qBound(0, endY, originalImage.height());
        QRect rect(0, startY, originalImage.width(), endY - startY);
        rects.append(rect);
    }

    // Обрабатываем каждую часть изображения в отдельном потоке
    QFuture<void> future = QtConcurrent::map(rects, [&](const QRect &rect) {
        adjustImagePart(originalImage, adjustedImage, rect);
    });
    future.waitForFinished();

    pixmapItem->setPixmap(QPixmap::fromImage(adjustedImage));
}

void MainWindow::adjustImagePart(const QImage &originalImage, QImage &adjustedImage, const QRect &rect)
{
    int startY = rect.top();
    int endY = rect.bottom();
    int startX = rect.left();
    int endX = rect.right();

    // Ограничиваем границы итерации
    startY = qMax(0, startY);
    endY = qMin(originalImage.height(), endY);
    startX = qMax(0, startX);
    endX = qMin(originalImage.width(), endX);

    for (int i = startY; i < endY; ++i) {
        for (int j = startX; j < endX; ++j) {
            QColor color = originalImage.pixelColor(j, i);

            // Применяем яркость
            int red = qBound(0, color.red() + brightnessValue, 255);
            int green = qBound(0, color.green() + brightnessValue, 255);
            int blue = qBound(0, color.blue() + brightnessValue, 255);
            color = QColor(red, green, blue);

            // Применяем цветовой тон
            int hue, sat, val;
            color.getHsv(&hue, &sat, &val);
            hue += hueValue;
            if (hue < 0) {
                hue = (hue + 360) % 360;
            } else if (hue >= 360) {
                hue -= 360;
            }
            color = QColor::fromHsv(hue, sat, val);

            // Применяем насыщенность
            int newSaturation = qBound(0, color.saturation() + saturationValue, 255);
            color = QColor::fromHsv(color.hue(), newSaturation, color.value());

            adjustedImage.setPixelColor(j, i, color);
        }
    }
}



void MainWindow::adjustBrightness(int value)
{
    brightnessValue = value;
    updateImage();
}

void MainWindow::adjustHue(int value)
{
    hueValue = value;
    updateImage();
}

void MainWindow::adjustSaturation(int value)
{
    saturationValue = value;
    updateImage();
}

void MainWindow::enableHandTool()
{
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::open()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!imagePath.isEmpty()) {
        QImage image(imagePath);
        if (!image.isNull()) {
            brightnessValue = 0;
            hueValue = 0;
            saturationValue = 0;

            originalPixmap = QPixmap::fromImage(image);
            ui->graphicsView->setScene(nullptr);
            ui->graphicsView->setScene(new QGraphicsScene(this));
            pixmapItem = ui->graphicsView->scene()->addPixmap(originalPixmap); // Сохраняем pixmapItem
            ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open image."));
        }
    }
}

void MainWindow::save()
{
    QString imagePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Files (*.png);;JPEG Files (*.jpg);;Bitmap Files (*.bmp)"));
    if (!imagePath.isEmpty()) {
        QImage tempImage(ui->graphicsView->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
        tempImage.fill(Qt::transparent);
        QPainter painter(&tempImage);
        ui->graphicsView->scene()->render(&painter);
        painter.end();

        if (!tempImage.isNull()) {
            if (!tempImage.save(imagePath)) {
                QMessageBox::warning(this, tr("Error"), tr("Unable to save image."));
            }
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Unable to save image."));
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsView->viewport()) {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            wheelZoom(wheelEvent);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::wheelZoom(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        QPointF targetViewportPos = ui->graphicsView->mapToScene(event->pos());
        int angle = event->angleDelta().y();
        qreal scaleFactor = 1.15;
        if (angle < 0) {
            scaleFactor = 1.0 / scaleFactor;
        }
        ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        ui->graphicsView->scale(scaleFactor, scaleFactor);
        QPointF deltaViewportPos = targetViewportPos - ui->graphicsView->mapToScene(event->pos());
        ui->graphicsView->horizontalScrollBar()->setValue(deltaViewportPos.x() + ui->graphicsView->horizontalScrollBar()->value());
        ui->graphicsView->verticalScrollBar()->setValue(deltaViewportPos.y() + ui->graphicsView->verticalScrollBar()->value());
        event->accept();
    }
}
