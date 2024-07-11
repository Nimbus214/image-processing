// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void adjustBrightness(int value);
    void adjustHue(int value);
    void adjustSaturation(int value);
    void enableHandTool();
    void rotateImageClockwise();
    void rotateImageCounterClockwise();
    void open();
    void save();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPixmap originalPixmap;
    QImage originalImage;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    void wheelZoom(QWheelEvent *event);
    int brightnessValue = 0;
    int hueValue = 0;
    int saturationValue = 0;
    void updateImage();
    void adjustImagePart(const QImage &originalImage, QImage &adjustedImage, const QRect &rect); // Объявление функции adjustImagePart
};

#endif // MAINWINDOW_H
