#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class Painter
{
public:
    // цвет
    Painter();
public:
    void draw(QPainter* painter, QPaintEvent* event); // связь между вершинами и ребром
    void draw_top(int x, int y, QPainter* painter, QString name); // рисует вершину на заданных координатах x и y
    void draw_edge(int x1, int y1, int x2,int y2, QPainter* painter, int weight, int type); // рисует ребро между двумя заданными точками, например, (x1, y1) и (x2, y2)
    QBrush shape;
private:
    QBrush background;
    QPen textPen;
    QFont glFont;
};

#endif // MYPAINTER_H
