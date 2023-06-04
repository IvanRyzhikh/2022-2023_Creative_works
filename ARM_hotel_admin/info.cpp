#include "info.h"
#include "ui_info.h"
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QString>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDebug>

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

    // дата сегоднящнего дня
    ui->Birth_date_push->setDate(QDate::currentDate());
    ui->Checkin_date_push->setDate(QDate::currentDate());
    ui->Eviction_date_push->setDate(QDate::currentDate());

    // ограничения на выбор комнат
    ui->Room_push->setMinimum(1);
    ui->Room_push->setMaximum(20);
}

// деструктор
Info::~Info()
{
    delete ui;
}

// Обработчик события нажатия на кнопку "Save"
void Info::on_Save_clicked()
{
    QString room = ui->Room_push->text();
    QString familyName = ui->F_name_push->text();
    QString name = ui->Name_push->text();
    QString surname = ui->Surname_push->text();
    QDate birthDate = ui->Birth_date_push->date();
    QDate checkInDate = ui->Checkin_date_push->date();
    QDate evictionDate = ui->Eviction_date_push->date();

    bool validRoomNumber = false;
    int roomNumber = room.toInt(&validRoomNumber);
    if (!validRoomNumber || roomNumber < 1 || roomNumber > 20)
    {
        return;
    }

    // создание файла, название которого будет зависить от номера room
    QString fileName = "room_" + QString::number(roomNumber) + ".txt";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        // вывод в текстовый документ
        QTextStream stream(&file);
        stream << "Family name: " << familyName << endl;
        stream << "Name: " << name << endl;
        stream << "Surname: " << surname << endl;
        stream << "Birthday date: " << birthDate.toString("dd.MM.yyyy") << endl;
        stream << "Check-in date: " << checkInDate.toString("dd.MM.yyyy") << endl;
        stream << "Eviction date: " << evictionDate.toString("dd.MM.yyyy") << endl;
        stream << endl;
        file.close();

        // очистка
        ui->Room_push->clear();
        ui->F_name_push->clear();
        ui->Name_push->clear();
        ui->Surname_push->clear();
        ui->Birth_date_push->setDate(QDate::currentDate());
        ui->Checkin_date_push->setDate(QDate::currentDate());
        ui->Eviction_date_push->setDate(QDate::currentDate());
    }
}

// Обработчик события нажатия на кнопку "View"
void Info::on_View_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

        QFile file(fileName);
        // только для чтения и в виде текстового файла
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            // отображение в консоли
            while (!in.atEnd())
            {
                QString line = in.readLine();
                qDebug().noquote() << line;
            }
            file.close();
        }
    }
}

// Обработчик события нажатия на кнопку "OK"
void Info::on_pushButton_clicked()
{
    ui->Room_push->clear();
    ui->F_name_push->clear();
    ui->Name_push->clear();
    ui->Surname_push->clear();
    ui->Birth_date_push->setDate(QDate::currentDate());
    ui->Checkin_date_push->setDate(QDate::currentDate());
    ui->Eviction_date_push->setDate(QDate::currentDate());
    reject();
}

// Обработчик события нажатия на кнопку "Cancel"
void Info::on_pushButton_2_clicked()
{
    ui->Room_push->clear();
    ui->F_name_push->clear();
    ui->Name_push->clear();
    ui->Surname_push->clear();
    ui->Birth_date_push->setDate(QDate::currentDate());
    ui->Checkin_date_push->setDate(QDate::currentDate());
    ui->Eviction_date_push->setDate(QDate::currentDate());
    reject();
}

