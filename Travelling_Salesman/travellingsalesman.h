#ifndef TRAVELLINGSALESMAN_H
#define TRAVELLINGSALESMAN_H

#include <QMainWindow>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

namespace Ui { class TSP; }

QT_END_NAMESPACE

class TSP : public QMainWindow {
    Q_OBJECT
public:
    TSP(QWidget *parent = nullptr);
    ~TSP();
    GlWidget* openGlW;
private slots:
    void on_Btn_Add_Top_clicked(); // добавление вершины
    void on_Btn_Del_Top_clicked(); // удаление вершины
    void on_Btn_Add_Edge_2_clicked(); // добавление ребра
    void on_Btn_remove_Edge_clicked(); // удаление ребра
    void on_Btn_Calculate_clicked(); // подсчет кратчейшего пути
    void on_L_Clear_clicked(); // очистка виджета
private:
    Ui::TSP *ui;
};
#endif // TRAVELLINGSALESMAN_H
