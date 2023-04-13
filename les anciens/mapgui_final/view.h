#pragma once

#include <QFrame>
#include <QGraphicsView>
#include <QLineEdit>
QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v) : QGraphicsView(), view(v) { }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

private:
    View *view;
};

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;
  int take_int();
   int take_int2();
public slots:
    void zoomIn();
    void zoomOut();
    void zoomInBy(int level);
    void zoomOutBy(int level);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void selectAlgorithm();
    void rotateLeft();
    void rotateRight();

private:
    GraphicsView *graphicsView;
    QLabel *label;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLineEdit *lineEdit;
     QLineEdit *lineEdit2;
    QToolButton *selectBFSButton_;
    QToolButton *selectDijkstraButton_;
    QToolButton *selectAstarButton_;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
};
