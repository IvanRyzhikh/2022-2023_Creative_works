#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include "painter.h"

class GlWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GlWidget(QWidget *parent);
    void redraw(); // перерисовка
    void paintEvent(QPaintEvent* event) override; // рисование на виджете
private:
   Painter* mypainter;
};

#endif // MYGLWIDGET_H
