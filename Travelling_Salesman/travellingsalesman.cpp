#include "travellingsalesman.h"
#include "ui_travellingsalesman.h"
#include "vertex.h"
#include "edge.h"

#include <QPainter>
#include <QtMath>
#include <QIntValidator>

const int graph_R = 200;
const int shift_x = 250;
const int shift_y = 250;
const double pi = 3.1415;
const int max_knot_count = 8;

extern QVector<TOP> tops;
extern QVector<EDGE> edges;
extern QVector<QVector<int>> map;

bool flag[max_knot_count] = {0, 0, 0, 0, 0, 0, 0, 0};
int knot_count = 0;

// инициализирует графический интерфейс пользователя и настраивает элементы интерфейса.
TSP::TSP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TSP)
{
    ui->setupUi(this);
    openGlW = new GlWidget(this);
    ui->verticalLayout->addWidget(openGlW, 0, 0);
    ui->LE_Del_Top_index->setValidator(new QIntValidator(1,8,this));
    ui->LE_Add_Edge_2_Top_1->setValidator(new QIntValidator(1,8,this));
    ui->LE_Add_Edge_2_Top_2->setValidator(new QIntValidator(1,8,this));
    ui->LE_Add_Edge_2_Weight->setValidator(new QIntValidator(1,99,this));
    ui->LE_Remove_Edge_Top_1->setValidator(new QIntValidator(1,8,this));
    ui->LE_Remove_Edge_Top_2->setValidator(new QIntValidator(1,8,this));
}

// деструктор
TSP::~TSP() { delete ui; }

// Обработка событий для кнопки "add Vertex"
void TSP::on_Btn_Add_Top_clicked() {
    ui->L_Result->setText("");
    ui->L_Path_length->setText("");
    ui->L_Errors->setText("");
    if(knot_count != max_knot_count) {
        int id = 0;
        while(flag[id] != 0 && id < 7) {
            id++;
        }
        flag[id] = 1;
        double angel = pi * 3/2 + pi * 2 / max_knot_count * id;
        int knot_x = cos(angel) * graph_R + shift_x;
        int knot_y = sin(angel) * graph_R + shift_y;
        TOP top(knot_x, knot_y, id);
        tops.push_back(top);
        knot_count++;
        QVector<int> row(knot_count);
        row.fill(0);
        map.append(row);
        for(int i = 0; i < knot_count - 1; i++) {
            map[i].append(0);
        }
        openGlW->redraw();
    }
}

// Обработка событий для кнопки "remove Vertex"
void TSP::on_Btn_Del_Top_clicked() {
    ui->L_Errors->setText("");
    ui->L_Path_length->setText("");
    ui->L_Errors->setText("");
    if(knot_count != 0) {
        QString del = ui->LE_Del_Top_index->text();
        int del_index = del.toInt()-1;
        if(del_index == -1) {
            ui->L_Errors->setText("Fill in the empty fields to delete a vertex!");
        }
        else if(del_index+1>knot_count) {
            ui->L_Errors->setText("There is no vertex entered in the graph!");
        }
        else {
            TOP del_top = tops[del_index];
            flag[del_top.pos] = 0;
            tops.erase(tops.begin() + del_index);
            QVector <EDGE> new_edges;
            for(int i = 0; i < edges.size(); i++) {
                if(!(edges[i].x_from == del_top.x && edges[i].y_from == del_top.y) && !(edges[i].x_to == del_top.x && edges[i].y_to == del_top.y)) {
                    new_edges.push_back(edges[i]);
                }
            }
            edges=new_edges;
            for(int i = 0; i < knot_count; i++) {
                map[i].erase(map[i].begin() + del_index);
            }
            map.erase(map.begin() + del_index);
            knot_count--;
            openGlW->redraw();
        }
    }
    ui->LE_Del_Top_index->setText("");
}

// Обработка событий для кнопки "add edge"
void TSP::on_Btn_Add_Edge_2_clicked() {
    ui->L_Errors->setText("");
    ui->L_Path_length->setText("");
    ui->L_Errors->setText("");
    if(tops.size()>1) {
        QString new_edge = ui->LE_Add_Edge_2_Top_1->text();
        int from = new_edge.toInt()-1;
        new_edge = ui->LE_Add_Edge_2_Top_2->text();
        int to = new_edge.toInt()-1;
        new_edge = ui->LE_Add_Edge_2_Weight->text();
        int weight = new_edge.toInt();
        if(from == -1 || to == -1 || weight == 0) {
            ui->L_Errors->setText("Fill in the empty fields to add an edge!");
        }
        else if(from + 1 > knot_count || to + 1 > knot_count) {
            ui->L_Errors->setText("There is no vertex entered in the graph!");
        }
        else {
            EDGE edge(tops[from].x, tops[from].y, tops[to].x, tops[to].y, weight, 0);
            edges.push_back(edge);
            map[from][to] = weight;
            map[to][from] = weight;
            openGlW->redraw();
        }
    }
    ui->LE_Add_Edge_2_Top_1->setText("");
    ui->LE_Add_Edge_2_Top_2->setText("");
    ui->LE_Add_Edge_2_Weight->setText("");
}

// Обработка событий для кнопки "Calculate the optimal path"
void TSP::on_Btn_Calculate_clicked() {
    ui->L_Errors->setText("");
    try {
        int X = 1000;
        QVector<QVector<int>> matrix = map;
        for(int i = 0; i < knot_count; i++) {
            for(int j = 0; j < knot_count; j++) {
                if(i==j || matrix[i][j]==0) {
                    matrix[i][j] = X;
                }
            }
        }
        QVector<int> from;
        QVector<int> to;
        bool calculation_flag = true;
        int sum = 0;
        while(calculation_flag) {
            //Редукция строк и столбцов
            for(int i = 0; i < knot_count; i++) {
                int row_min = X;
                for(int j = 0; j < knot_count; j++) {
                    row_min=qMin(row_min, matrix[i][j]);
                }
                for(int j = 0; j < knot_count; j++) {
                    if(row_min != X && matrix[i][j] != X) {
                        matrix[i][j] -= row_min;
                    }
                }
            }
            for(int i = 0; i < knot_count; i++) {
                int column_min = X;
                for(int j = 0; j < knot_count; j++) {
                    column_min=qMin(column_min, matrix[j][i]);
                }
                for(int j = 0; j < knot_count; j++) {
                    if(column_min != X && matrix[j][i] != X) {
                        matrix[j][i] -= column_min;
                    }
                }
            }
            //Оценка нулей
            int max = -1;
            int max_x;
            int max_y;
            for(int i = 0; i < knot_count; i++) {
                for (int j = 0; j < knot_count; j++) {
                    if(matrix[i][j] == 0) {
                        int row_min = X;
                        for(int k = 0; k < knot_count; k++) {
                            if(k != j) {
                                row_min = qMin(row_min, matrix[i][k]);
                            }
                        }
                        int column_min = X;
                        for(int k = 0; k < knot_count; k++) {
                            if(k != i) {
                                column_min = qMin(column_min, matrix[k][j]);
                            }
                        }
                        if(row_min + column_min > max) {
                            max = row_min + column_min;
                            max_x = i;
                            max_y = j;
                        }
                    }
                }
            }
            sum += map[max_x][max_y];
            matrix[max_y][max_x] = X;
            for(int i = 0; i < knot_count; i++) {
                matrix[max_x][i] = X;
                matrix[i][max_y] = X;
            }
            from.push_back(max_x + 1);
            to.push_back(max_y + 1);
            //Проверка
            calculation_flag = false;
            for(int i = 0; i < knot_count; i++) {
                for(int j = 0; j < knot_count; j++) {
                    if(matrix[i][j] != X) {
                        calculation_flag = true;
                    }
                }
            }
        }
        //Построение маршрута
        QString path = QString::number(from[0]) + " -> " + QString::number(to[0]);
        int new_from = to[0];
        from.erase(from.begin());
        to.erase(to.begin());
        while(from.size() != 0) {
            for(int i = 0; i < from.size(); i++) {
                if(from[i] == new_from) {
                    path += " -> " + QString::number(to[i]);
                    new_from = to[i];
                    from.erase(from.begin() + i);
                    to.erase(to.begin() + i);
                }
            }
        }
        ui->L_Result->setText(path);
        QString result = "Path length = ";
        result += QString::number(sum);
        ui->L_Path_length->setText(result);
    }
    catch(...) {
        ui->L_Result->setText("Error: It is impossible to calculate the optimal path");
    }
    ui->LE_Del_Top_index->setText("");
    ui->LE_Add_Edge_2_Top_1->setText("");
    ui->LE_Add_Edge_2_Top_2->setText("");
    ui->LE_Add_Edge_2_Weight->setText("");
    ui->LE_Remove_Edge_Top_1->setText("");
    ui->LE_Remove_Edge_Top_2->setText("");
}

// Обработка событий для кнопки "Clear all"
void TSP::on_L_Clear_clicked() {
    knot_count=0;
    tops.clear();
    tops.squeeze();
    edges.clear();
    edges.squeeze();
    map.clear();
    map.squeeze();
    for(int i = 0; i < max_knot_count; i++) {
        flag[i]=0;
    }
    ui->L_Errors->setText("");
    ui->LE_Del_Top_index->setText("");
    ui->LE_Add_Edge_2_Top_1->setText("");
    ui->LE_Add_Edge_2_Top_2->setText("");
    ui->LE_Add_Edge_2_Weight->setText("");
    ui->L_Result->setText("");
    ui->L_Path_length->setText("");
    ui->LE_Remove_Edge_Top_1->setText("");
    ui->LE_Remove_Edge_Top_2->setText("");
    openGlW->redraw();
}

// Обработка событий для кнопки "remove edge"
void TSP::on_Btn_remove_Edge_clicked() {
    ui->L_Errors->setText("");
    ui->L_Path_length->setText("");
    ui->L_Errors->setText("");

    if (edges.size() > 0) {
        QString remove_edge = ui->LE_Remove_Edge_Top_1->text();
        int from = remove_edge.toInt() - 1;
        remove_edge = ui->LE_Remove_Edge_Top_2->text();
        int to = remove_edge.toInt() - 1;

        if (from == -1 || to == -1) {
            ui->L_Errors->setText("Fill in the empty fields to remove an edge!");
        }
        else if (from >= knot_count || to >= knot_count) {
            ui->L_Errors->setText("There is no vertex entered in the graph!");
        }
        else {

            // Поиск и удаление ребра
            int index = -1;
            for (int i = 0; i < edges.size(); i++) {
                if ((edges[i].x_from == tops[from].x && edges[i].y_from == tops[from].y) &&
                    (edges[i].x_to == tops[to].x && edges[i].y_to == tops[to].y))
                {
                    index = i;
                    break;
                }
            }

            if (index != -1) {
                edges.remove(index);
                map[from][to] = 0;
                map[to][from] = 0;
                openGlW->redraw();
            }
        }
    }
    ui->LE_Remove_Edge_Top_1->setText("");
    ui->LE_Remove_Edge_Top_2->setText("");
}
