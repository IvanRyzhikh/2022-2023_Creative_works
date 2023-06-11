#include "glwidget.h"
#include "vertex.h"
#include "edge.h"

#include <QPainter>

extern QVector<TOP> tops;
extern QVector<EDGE> edges;

GlWidget::GlWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{
    mypainter=new Painter;
}

// перерисовки виджета
void GlWidget::redraw() { this->update(); }

// позволяет рисовать на виджете
void GlWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    mypainter->draw(&painter,event);
    painter.end();
}
