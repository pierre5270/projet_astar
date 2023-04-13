// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "view.h"
#include <QLineEdit>
#include <QtWidgets>
#include <QtMath>
#include "graph.h"
#include "vertexitem.h"
#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->angleDelta().y() > 0)
            view->zoomInBy(6);
        else
            view->zoomOutBy(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif

View::View(const QString &name, QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    label = new QLabel(name);
    label2 = new QLabel(tr("Algorithm "));
    label3 =new QLabel(tr("Vbegin"));
    label4 =new QLabel(tr("Vend"));

    lineEdit = new QLineEdit;
    lineEdit2 = new QLineEdit;
    selectBFSButton_ = new QToolButton;
    selectBFSButton_->setText(tr("BFS"));
    selectBFSButton_->setCheckable(true);
    selectBFSButton_->setChecked(true);

    selectDijkstraButton_ = new QToolButton;
    selectDijkstraButton_->setText(tr("Dijkstra"));
    selectDijkstraButton_->setCheckable(true);
    selectDijkstraButton_->setChecked(false);

    selectAstarButton_ = new QToolButton;
    selectAstarButton_->setText(tr("Astar"));
    selectAstarButton_->setCheckable(true);
    selectAstarButton_->setChecked(false);

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectBFSButton_);
    pointerModeGroup->addButton(selectDijkstraButton_);
    pointerModeGroup->addButton(selectAstarButton_);

    labelLayout->addWidget(label);
    labelLayout->addStretch();
    labelLayout->addWidget(label2);
    labelLayout->addWidget(label3);
    labelLayout->addWidget(label4);
    labelLayout->addWidget(lineEdit);
    labelLayout->addWidget(lineEdit2);
    labelLayout->addWidget(selectBFSButton_);
    labelLayout->addWidget(selectDijkstraButton_);
    labelLayout->addWidget(selectAstarButton_);
    labelLayout->addStretch();

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);
    QPushButton *button = new QPushButton("Valider");
    labelLayout->addWidget(button);

     connect(button, &QAbstractButton::clicked,this,&View::take_int);
     connect(button, &QAbstractButton::clicked,this,&View::take_int2);

    connect(resetButton, &QAbstractButton::clicked, this, &View::resetView);
    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &View::setupMatrix);
    connect(rotateSlider, &QAbstractSlider::valueChanged, this, &View::setupMatrix);
    connect(graphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged,
            this, &View::setResetButtonEnabled);
    connect(graphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged,
            this, &View::setResetButtonEnabled);
    connect(selectBFSButton_, &QAbstractButton::toggled, this, &View::selectAlgorithm);
    connect(selectDijkstraButton_, &QAbstractButton::toggled, this, &View::selectAlgorithm);
    connect(selectAstarButton_, &QAbstractButton::toggled, this, &View::selectAlgorithm);
    connect(rotateLeftIcon, &QAbstractButton::clicked, this, &View::rotateLeft);
    connect(rotateRightIcon, &QAbstractButton::clicked, this, &View::rotateRight);
    connect(zoomInIcon, &QAbstractButton::clicked, this, &View::zoomIn);
    connect(zoomOutIcon, &QAbstractButton::clicked, this, &View::zoomOut);

    setupMatrix();
}

QGraphicsView *View::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void View::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    graphicsView->setTransform(matrix);
    setResetButtonEnabled();
}
int View::take_int(){
    QString sss=lineEdit->text();
    int val_i=sss.toInt();
    //cout<<val_i<<endl;$
    return val_i;
}
int View::take_int2(){
    QString sss=lineEdit2->text();
    int val_i=sss.toInt();
    //cout<<val_i<<endl;$
    return val_i;
}
void View::selectAlgorithm()
{QMouseEvent *event;

    //QGraphicsScene sc=new QGraphicsScene(this);

  if (selectBFSButton_->isChecked()) {
    qDebug() << "Algorithm BFS is selected";
   int vbegin=take_int();
   int vend=take_int2();
   vector<uint32_t> tab_set;
   vector<uint32_t> tab_queue;

 //  cout<<"vbegin="<<vbegin<<" et vend="<<vend<<endl;

//cout<<val_i<<endl;

    string file="graph_dc_area.txt";
    string file2="./mapdata.txt";
    Graph graph(file);
//VertexItem *ii=     mousePressEvent(event);
    //cout<<event->position();

   graph.bfs(vbegin , vend,&tab_set,&tab_queue );
    double x1=graph.to_cartesianX(vbegin);
    double  y1=graph.to_cartesianY(vbegin);
    double x2=graph.to_cartesianX(vend);
    double  y2=graph.to_cartesianY(vend);

   // cout<<tab<<endl;
    string gr=to_string(vbegin)+ " "+to_string(x1)+ " "+to_string(y1);
    QGraphicsItem *item = new VertexItem(gr);
         QPainter painter;
        painter.setPen(QPen(Qt::white, 2));
        painter.setBrush(Qt::red);
        QStyleOptionGraphicsItem option;
        option.initFrom(this);
        option.state |= QStyle::State_Sunken;
        option.palette.setColor(QPalette::Base, Qt::red);

    item->paint(&painter,&option);

    this->view()->scene()->addItem(item);
  } else {
    qDebug() << "Algorithm BFS is not selected";
  }


  if (selectDijkstraButton_->isChecked()) {
    qDebug() << "Algorithm Dijkstra is selected";
  } else {
    qDebug() << "Algorithm Dijkstra is not selected";
  }

  if (selectAstarButton_->isChecked()) {
    qDebug() << "Algorithm Astar is selected";
  } else {
    qDebug() << "Algorithm Astar is not selected";
  }
}

void View::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void View::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void View::zoomInBy(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOutBy(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void View::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void View::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}
