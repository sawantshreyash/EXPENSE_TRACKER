#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Expense.h"
#include <vector>
#include <QString>

class FileManager
{
public:
    static bool saveExpenses(const std::vector<Expense>& expenses,
                             const QString& filename);

    static std::vector<Expense> loadExpenses(
        const QString& filename);
};

#endif // FILEMANAGER_H