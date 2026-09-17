#ifndef EXPENSEMANAGER_H
#define EXPENSEMANAGER_H

#include "Expense.h"
#include <vector>
#include <QString>

class ExpenseManager
{
private:
    std::vector<Expense> expenses;

public:
    ExpenseManager();

    void loadFromFile(const QString& filename);

    bool addExpense(const Expense& expense,
                    const QString& filename);

    bool updateExpense(const Expense& expense,
                       const QString& filename);

    bool deleteExpense(int id,
                       const QString& filename);

    std::vector<Expense> getExpenses() const;

    Expense* getExpenseById(int id);

    int getNextId() const;

    double getTotalExpenses() const;

    double getTotalForMonth(const QString& month) const;
};

#endif // EXPENSEMANAGER_H