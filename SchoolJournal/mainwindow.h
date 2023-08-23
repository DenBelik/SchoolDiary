#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDate>
#include <QString>
#include <QVector>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    struct ScheduleList {
        QString lesson[8];
    };

    QVector<ScheduleList> scheduleList;

    struct Comment {
        QString date;
        QString comment[8];
        QString grade[8];
    };

    QVector<Comment> commentList;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDayOfWeek(int dayOfWeek);

    void loadScheduleFromFile(QFile &file, QVector<ScheduleList> &scheduleList);
    void saveSchedule(QVector<ScheduleList> &list, int dayOfWeek);
    void saveScheduleToFile(QFile &file, QVector<ScheduleList> &list);

    void loadCommentListFromFile(QFile &file, QVector<Comment> &commentList);
    void saveCommentList(QVector<Comment> &list, QString date);
    void saveCommentListToFile(QFile &file, QVector<Comment> &commentList);

    void reloadTable(QVector<ScheduleList> &list, QVector<Comment> &commentsList, int dayOfWeek, QString date);

private slots:
    void on_SaveBtn_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
