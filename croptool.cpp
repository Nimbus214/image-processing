#include "croptool.h"
#include <QGraphicsScene>

CropTool::CropTool(QGraphicsScene *scene, QObject *parent)
    : QObject(parent), m_scene(scene) // Инициализация переменной m_scene
{
    // Дополнительная инициализация, если необходимо
}

CropTool::~CropTool()
{
    if (cropRectItem) {
        delete cropRectItem;
    }
}

void CropTool::startCrop(const QPointF &startPoint)
{
    this->startPoint = startPoint;
    cropRectItem = new QGraphicsRectItem(startPoint.x(), startPoint.y(), 0, 0);
    cropRectItem->setPen(QPen(Qt::red, 2));
    cropRectItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    cropRectItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void CropTool::updateCrop(const QPointF &currentPoint)
{
    if (cropRectItem) {
        qreal width = currentPoint.x() - startPoint.x();
        qreal height = currentPoint.y() - startPoint.y();
        cropRectItem->setRect(startPoint.x(), startPoint.y(), width, height);
    }
}

void CropTool::endCrop()
{
    if (cropRectItem) {
        QRectF cropRect = cropRectItem->rect();
        emit cropFinished(cropRect);
    }
}
