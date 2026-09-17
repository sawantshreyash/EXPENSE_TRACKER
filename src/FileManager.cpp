#include "FileManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

bool FileManager::saveExpenses(
    const std::vector<Expense>& expenses,
    const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&file);

    out << "ID,Date,Category,Amount,Description\n";

    for (const Expense& expense : expenses)
    {
        out << expense.id << ","
            << expense.date << ","
            << expense.category << ","
            << expense.amount << ","
            << expense.description << "\n";
    }

    file.close();

    return true;
}

std::vector<Expense> FileManager::loadExpenses(
    const QString& filename)
{
    std::vector<Expense> expenses;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return expenses;
    }

    QTextStream in(&file);

    // Skip header
    if (!in.atEnd())
    {
        in.readLine();
    }

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.trimmed().isEmpty())
        {
            continue;
        }

        QStringList fields = line.split(",");

        if (fields.size() < 5)
        {
            continue;
        }

        Expense expense;

        expense.id = fields[0].toInt();
        expense.date = fields[1];
        expense.category = fields[2];
        expense.amount = fields[3].toDouble();
        expense.description = fields[4];

        expenses.push_back(expense);
    }

    file.close();

    return expenses;
}