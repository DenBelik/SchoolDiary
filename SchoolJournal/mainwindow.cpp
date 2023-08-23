#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scheduleList.resize(7);

    ui->dateEdit->setDate(QDate::currentDate());

    ui->tableWidget->setRowCount(8);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 400);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 100);

    QFile scheduleFile("schedule.csv");
    QFile commentsFile("comments.csv");

    loadScheduleFromFile(scheduleFile, scheduleList);
    loadCommentListFromFile(commentsFile, commentList);

    reloadTable(scheduleList, commentList, ui->dateEdit->date().dayOfWeek(), ui->dateEdit->date().toString());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDayOfWeek(int dayOfWeek) {
    switch (dayOfWeek) {
    case 1:
        ui->label->setText("Понедельник");
        break;
    case 2:
        ui->label->setText("Вторник");
        break;
    case 3:
        ui->label->setText("Среда");
        break;
    case 4:
        ui->label->setText("Четверг");
        break;
    case 5:
        ui->label->setText("Пятница");
        break;
    case 6:
        ui->label->setText("Суббота");
        break;
    case 7:
        ui->label->setText("Воскресенье");
        break;
    }
}

void MainWindow::loadScheduleFromFile(QFile &file, QVector<ScheduleList> &scheduleList) {
    int day;
    int i = 0;
    ScheduleList list;
    file.open(QIODevice::ReadWrite);
    while (!file.atEnd()) {
        i += 1;
           QString line = file.readLine();
           day = line.section(";", 0, 0).toInt();
           list.lesson[0] = line.section(";", 1, 1);
           list.lesson[1] = line.section(";", 2, 2);
           list.lesson[2] = line.section(";", 3, 3);
           list.lesson[3] = line.section(";", 4, 4);
           list.lesson[4] = line.section(";", 5, 5);
           list.lesson[5] = line.section(";", 6, 6);
           list.lesson[6] = line.section(";", 7, 7);
           list.lesson[7] = line.section(";", 8, 8);
           if (day < 7) scheduleList.replace(day, list);

    }
    file.close();
}

void MainWindow::saveSchedule(QVector<ScheduleList> &list, int dayOfWeek) {

    for (int i = 0; i < 7; i++) {

        if (ui->tableWidget->item(i,0) != 0) {
            list[dayOfWeek].lesson[i] = ui->tableWidget->item(i, 0)->text();
        }
    }
}

void MainWindow::saveScheduleToFile(QFile &file, QVector<ScheduleList> &list) {
    file.open(QIODevice::WriteOnly);
    QTextStream output(&file);
    for (int i = 0; i < list.size(); i++) {
        output << i << ";"
               << list[i].lesson[0] << ";"
               << list[i].lesson[1] << ";"
               << list[i].lesson[2] << ";"
               << list[i].lesson[3] << ";"
               << list[i].lesson[4] << ";"
               << list[i].lesson[5] << ";"
               << list[i].lesson[6] << ";"
               << list[i].lesson[7] << "\n";
    }
}

void MainWindow::loadCommentListFromFile(QFile &file, QVector<MainWindow::Comment> &commentList)
{
    Comment emptyComment;
    file.open(QIODevice::ReadWrite);
    while(!file.atEnd()) {
        QString line = file.readLine();
        //qDebug() << line.section(";", 0, 0);
        emptyComment.date = line.section(";", 0, 0);
        for (int i = 0; i < 7; i++) {
            emptyComment.comment[i] = line.section(";", i+1, i+1);
        }
        for (int i = 0; i < 7; i++) {
            emptyComment.grade[i] = line.section(";", i+8, i+8);
        }
        commentList.push_back(emptyComment);
    }
    file.close();
}

void MainWindow::saveCommentList(QVector<MainWindow::Comment> &list, QString date)
{
    Comment emptyComment;
    emptyComment.date = date;
    for (int i = 0; i < 7; i++) {
        if (ui->tableWidget->item(i, 1) != 0) emptyComment.comment[i] = ui->tableWidget->item(i, 1)->text();
        if (ui->tableWidget->item(i, 2) != 0) emptyComment.grade[i] = ui->tableWidget->item(i, 2)->text();
    }
    if (!list.empty()) {
        for (int i = 0; i < list.size(); i++) {
            if (list[i].date == emptyComment.date) {
                list.replace(i, emptyComment);
                break;
            }
        }
        list.push_back(emptyComment);
    } else {
        list.push_back(emptyComment);
    }
}

void MainWindow::saveCommentListToFile(QFile &file, QVector<Comment> &commentList) {
    file.open(QIODevice::ReadWrite);
    QTextStream out(&file);
    for (int i = 0; i < commentList.size(); i++) {
        out << commentList[i].date << ";";
        for (int j = 0; j < 7; j++) {
            out << commentList[i].comment[j] << ";";
        }
        for (int k = 0; k < 7; k++) {
            out << commentList[i].grade[k] << ";";
        }
        out << "\n";
    }
    file.close();
}

void MainWindow::reloadTable(QVector<MainWindow::ScheduleList> &list, QVector<Comment> &commentsList, int dayOfWeek, QString date)
{
    ui->tableWidget->clear();
    if (ui->dateEdit->date().dayOfWeek() != 7) {
        if (dayOfWeek == ui->dateEdit->date().dayOfWeek()) {
            for (int i = 0; i < 7; i++) {
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(list[dayOfWeek].lesson[i]));
            }
        }
        for (int j = 0; j < commentsList.size(); j++) {

            qDebug() << commentsList[j].date << "\n";
            if (commentsList[j].date == date) qDebug() << "true\n";
            else qDebug() << "false\n";

            if (date == commentsList[j].date) {
                for (int k = 0; k < 7; k++) {
                    ui->tableWidget->setItem(k, 1, new QTableWidgetItem(commentsList[j].comment[k]));
                    ui->tableWidget->setItem(k, 2, new QTableWidgetItem(commentsList[j].grade[k]));
                }
            }
        }
    }
}


void MainWindow::on_SaveBtn_clicked()
{
    QFile scheduleFile("schedule.csv");
    QFile commentsFile("comments.csv");

    if (ui->dateEdit->date().dayOfWeek() != 7) {
        saveSchedule(scheduleList, ui->dateEdit->date().dayOfWeek());
        saveScheduleToFile(scheduleFile, scheduleList);
        saveCommentList(commentList, ui->dateEdit->date().toString("dd.MM.yyyy"));
        saveCommentListToFile(commentsFile, commentList);
    }
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    qDebug() << ui->dateEdit->date().toString();
    reloadTable(scheduleList, commentList, ui->dateEdit->date().dayOfWeek(), ui->dateEdit->date().toString("dd.MM.yyyy"));
    setDayOfWeek(ui->dateEdit->date().dayOfWeek());
}
