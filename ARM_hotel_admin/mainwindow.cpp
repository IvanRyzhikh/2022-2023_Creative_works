#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QAxObject>
#include <QAxWidget>
#include <QApplication>
#include <QTextCodec>

#include <QChar>


QString file("C:\\example\\ExcelFile.xls");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // кодировка для qDebug
    QTextCodec *russiancodec = QTextCodec::codecForName("Cp1251");
    QTextCodec::setCodecForLocale(russiancodec);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Обработчик события нажатия на кнопку "Booking"
void MainWindow::on_pushButton_clicked()
{
    Info *infoDialog = new Info(this); // Создание объекта класса Info
    infoDialog->show(); // Отображение диалогового окна
}

// Обработчик события нажатия на кнопку "Report"
void MainWindow::on_pushButton_2_clicked()
{
    QStringList fileNames; // Список имен файлов для объединения

    // Цикл для генерации имен файлов и добавления их в список
    for (int i = 1; i <= 20; ++i)
    {
        QString fileName = "room_" + QString::number(i) + ".txt";
        fileNames << fileName;
    }

    QString outputFileName = "Report.xlsx"; // Имя выходного файла для объединенного содержимого

    // создание Excel файла
    QAxObject* excel = new QAxObject("Excel.Application"); // Создаем объект Excel
    excel->dynamicCall("SetVisible(bool)", false); // Устанавливаем видимость Excel

    QAxObject* workbooks = excel->querySubObject("Workbooks"); // Получаем коллекцию книг

    QAxObject* workbook = workbooks->querySubObject("Add"); // Создаем новую книгу
    QAxObject* worksheets = workbook->querySubObject("Worksheets"); // Получаем коллекцию листов

    QAxObject* worksheet = worksheets->querySubObject("Item(int)", 1); // Получаем первый лист
    QAxObject* range = worksheet->querySubObject("Range(const QString&)", "A1"); // Получаем ячейку A1

    int column = 1; // Счетчик столбцов

    // Цикл для перебора файлов и записи их содержимого в ячейки
    foreach (const QString& fileName, fileNames)
    {
        QFile inputFile(fileName);

        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream inputStream(&inputFile);
            QString fileContent = inputStream.readAll(); // Читаем содержимое файла
            inputFile.close();

            // Извлекаем номер комнаты из имени файла
            QString roomNumber = fileName.mid(5, fileName.indexOf('.') - 5);

            // Проверяем, находится ли введенный номер комнаты в диапазоне от 1 до 20
            bool validRoomNumber = false;
            int roomNum = roomNumber.toInt(&validRoomNumber);
            if (!validRoomNumber || roomNum < 1 || roomNum > 20)
            {
                // Показываем сообщение об ошибке или обрабатываем недопустимый номер комнаты
                continue; // Пропускаем этот файл и переходим к следующему
            }

            // Записываем имя файла в ячейку
            range = worksheet->querySubObject("Range(const QString&)", getColumnLabel(column) + "1");
            range->setProperty("Value", fileName);

            // Обрабатываем содержимое файла и записываем его в ячейки
            QStringList lines = fileContent.split('\n');
            int row = 2; // Начинаем со следующей доступной строки

            foreach (const QString& line, lines)
            {
                QString key = line.section(':', 0, 0).trimmed(); // Извлекаем ключ перед символом ":"
                QString value = line.section(':', 1).trimmed(); // Извлекаем значение после символа ":"

                if (!key.isEmpty() && !value.isEmpty())
                {
                    // Записываем ключ и значение в соответствующие столбцы
                    range = worksheet->querySubObject("Range(const QString&)", getColumnLabel(column) + QString::number(row));
                    range->setProperty("Value", key);

                    range = worksheet->querySubObject("Range(const QString&)", getColumnLabel(column + 1) + QString::number(row));
                    range->setProperty("Value", value);

                    row++;
                    // Переходим к следующей строке для записи второй информации
                    range = worksheet->querySubObject("Range(const QString&)", getColumnLabel(column) + QString::number(row));
                }
            }

            // Увеличиваем счетчик столбцов и добавляем пустой столбец между файлами
            column += 4;
        }
    }

    workbook->dynamicCall("SaveAs(const QString&)", outputFileName); // Сохраняем книгу
    workbook->dynamicCall("Close()"); // Закрываем книгу
    excel->dynamicCall("Quit()"); // Закрываем Excel

    delete excel;

    qDebug() << "Все файлы были объединены и сохранены в" << outputFileName;
}

QString MainWindow::getColumnLabel(int column)
{
    QString label;
    while (column > 0)
    {
        int remainder = (column - 1) % 26;
        label.prepend(QChar('A' + remainder));
        column = (column - 1) / 26;
    }
    return label;
}

