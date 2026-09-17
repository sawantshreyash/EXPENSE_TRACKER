#include "ExpenseManager.h"
#include "FileManager.h"

ExpenseManager::ExpenseManager()
{
}

void ExpenseManager::loadFromFile(const QString& filename)
{
    expenses = FileManager::loadExpenses(filename);
}

bool ExpenseManager::addExpense(
    const Expense& expense,
    const QString& filename)
{
    Expense newExpense = expense;

    newExpense.id = getNextId();

    expenses.push_back(newExpense);

    return FileManager::saveExpenses(expenses, filename);
}

bool ExpenseManager::updateExpense(
    const Expense& expense,
    const QString& filename)
{
    for (Expense& existingExpense : expenses)
    {
        if (existingExpense.id == expense.id)
        {
            existingExpense = expense;

            return FileManager::saveExpenses(
                expenses,
                filename
            );
        }
    }

    return false;
}

bool ExpenseManager::deleteExpense(
    int id,
    const QString& filename)
{
    for (auto it = expenses.begin();
         it != expenses.end();
         ++it)
    {
        if (it->id == id)
        {
            expenses.erase(it);

            return FileManager::saveExpenses(
                expenses,
                filename
            );
        }
    }

    return false;
}

std::vector<Expense> ExpenseManager::getExpenses() const
{
    return expenses;
}

Expense* ExpenseManager::getExpenseById(int id)
{
    for (Expense& expense : expenses)
    {
        if (expense.id == id)
        {
            return &expense;
        }
    }

    return nullptr;
}

int ExpenseManager::getNextId() const
{
    int maxId = 0;

    for (const Expense& expense : expenses)
    {
        if (expense.id > maxId)
        {
            maxId = expense.id;
        }
    }

    return maxId + 1;
}

double ExpenseManager::getTotalExpenses() const
{
    double total = 0.0;

    for (const Expense& expense : expenses)
    {
        total += expense.amount;
    }

    return total;
}

double ExpenseManager::getTotalForMonth(
    const QString& month) const
{
    double total = 0.0;

    for (const Expense& expense : expenses)
    {
        if (expense.date.startsWith(month))
        {
            total += expense.amount;
        }
    }

    return total;
}