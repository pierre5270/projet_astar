#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <cstdint>
#include <string>


class VertexItem : public QGraphicsItem
{
public:
    VertexItem(std::string const &line);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

#if 0
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
#endif

private:
    QColor color_;
    double x_;
    double y_;
    uint32_t vid_;
};
