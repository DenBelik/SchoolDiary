#ifndef SHCHEDULE_H
#define SHCHEDULE_H

#include <QFile>
#include <QVector>



class Schedule
{
public:
    Schedule();

private:
    int dayOfWeek;
    QString lesson [8];
};

#endif // SHCHEDULE_H
