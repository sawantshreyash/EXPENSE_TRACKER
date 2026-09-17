#ifndef EXPENSE_H
#define EXPENSE_H

#include <QString>

struct Expense
{
    int id;
    QString date;
    QString category;
    double amount;
    QString description;
};

#endif // EXPENSE_H