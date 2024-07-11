#ifndef CROPTOOL_H
#define CROPTOOL_H

#include <QObject>
#include <QGraphicsRectItem>

class CropTool : public QObject
{
    Q_OBJECT

public:
    explicit CropTool(QGraphicsScene *scene, QObject *parent = nullptr); // Обновленный конструктор
    ~CropTool();

    void startCrop(const QPointF &startPoint);
    void updateCrop(const QPointF &currentPoint);
    void endCrop();

public slots:
    void startCrop();

signals:
    void cropFinished(const QRectF &cropRect);

private:
    QGraphicsRectItem *cropRectItem;
    QPointF startPoint;
    QGraphicsScene *m_scene;
};

#endif // CROPTOOL_H
