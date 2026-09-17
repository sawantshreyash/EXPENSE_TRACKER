#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDate>

#include "ExpenseManager.h"

class ExpenseWindow : public QWidget
{
private:
    ExpenseManager manager;

    QDateEdit* dateEdit;
    QComboBox* categoryBox;
    QDoubleSpinBox* amountBox;
    QLineEdit* descriptionEdit;

    QTableWidget* expenseTable;

    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* saveButton;
    QPushButton* cancelButton;

    const QString fileName = "data/expenses.csv";

    int editingId = -1;

public:

    ExpenseWindow()
    {
        setWindowTitle("Expense Tracker");
        resize(1000, 700);

        createInterface();
        loadExpenses();
    }

private:

    void createInterface()
    {
        // TITLE
        QLabel* title = new QLabel("EXPENSE TRACKER");

        title->setStyleSheet(
            "font-size: 28px;"
            "font-weight: bold;"
            "padding: 15px;"
        );

        // DATE
        dateEdit = new QDateEdit(QDate::currentDate());

        dateEdit->setCalendarPopup(true);
        dateEdit->setDisplayFormat("yyyy-MM-dd");

        // CATEGORY
        categoryBox = new QComboBox();

        categoryBox->addItems({
            "Food",
            "Travel",
            "Shopping",
            "Education",
            "Health",
            "Bills",
            "Entertainment",
            "Other"
        });

        // AMOUNT
        amountBox = new QDoubleSpinBox();

        amountBox->setRange(
            0.01,
            10000000.00
        );

        amountBox->setDecimals(2);
        amountBox->setPrefix("₹ ");

        // DESCRIPTION
        descriptionEdit = new QLineEdit();

        descriptionEdit->setPlaceholderText(
            "Enter expense description"
        );

        // FORM
        QFormLayout* formLayout =
            new QFormLayout();

        formLayout->addRow(
            "Date:",
            dateEdit
        );

        formLayout->addRow(
            "Category:",
            categoryBox
        );

        formLayout->addRow(
            "Amount:",
            amountBox
        );

        formLayout->addRow(
            "Description:",
            descriptionEdit
        );

        // BUTTONS
        addButton =
            new QPushButton("Add Expense");

        editButton =
            new QPushButton("Edit Selected");

        deleteButton =
            new QPushButton("Delete Selected");

        saveButton =
            new QPushButton("Save Changes");

        cancelButton =
            new QPushButton("Cancel Edit");

        addButton->setMinimumHeight(40);
        editButton->setMinimumHeight(40);
        deleteButton->setMinimumHeight(40);
        saveButton->setMinimumHeight(40);
        cancelButton->setMinimumHeight(40);

        saveButton->setEnabled(false);
        cancelButton->setEnabled(false);

        // ------------------------------------------
        // BUTTON CONNECTIONS USING LAMBDAS
        // ------------------------------------------

        connect(
            addButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                addExpense();
            }
        );

        connect(
            editButton,P
            &QPushButton::clicked,
            this,
            [this]()
            {
                startEditing();
            }
        );

        connect(
            deleteButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                deleteExpense();
            }
        );

        connect(
            saveButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                saveEdit();
            }
        );

        connect(
            cancelButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                cancelEdit();
            }
        );

        // INPUT GROUP
        QGroupBox* inputGroup =
            new QGroupBox("Expense Details");

        QVBoxLayout* inputLayout =
            new QVBoxLayout();

        inputLayout->addLayout(formLayout);

        inputGroup->setLayout(
            inputLayout
        );

        // BUTTON LAYOUT
        QVBoxLayout* buttonLayout =
            new QVBoxLayout();

        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(cancelButton);

        // TABLE
        expenseTable =
            new QTableWidget();

        expenseTable->setColumnCount(5);

        expenseTable->setHorizontalHeaderLabels({
            "ID",
            "Date",
            "Category",
            "Amount",
            "Description"
        });

        expenseTable->setSelectionBehavior(
            QAbstractItemView::SelectRows
        );

        expenseTable->setSelectionMode(
            QAbstractItemView::SingleSelection
        );

        expenseTable->setEditTriggers(
            QAbstractItemView::NoEditTriggers
        );

        expenseTable->horizontalHeader()
            ->setSectionResizeMode(
                QHeaderView::Stretch
            );

        // MAIN LAYOUT
        QVBoxLayout* mainLayout =
            new QVBoxLayout();

        mainLayout->addWidget(title);
        mainLayout->addWidget(inputGroup);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(expenseTable);

        setLayout(mainLayout);
    }

    // ==========================================
    // LOAD
    // ==========================================

    void loadExpenses()
    {
        manager.loadFromFile(fileName);

        refreshTable();
    }

    // ==========================================
    // ADD
    // ==========================================

    void addExpense()
    {
        if (amountBox->value() <= 0)
        {
            QMessageBox::warning(
                this,
                "Invalid Amount",
                "Please enter a valid amount."
            );

            return;
        }

        Expense expense;

        expense.id = 0;

        expense.date =
            dateEdit->date()
                .toString("yyyy-MM-dd");

        expense.category =
            categoryBox->currentText();

        expense.amount =
            amountBox->value();

        expense.description =
            descriptionEdit->text();

        bool success =
            manager.addExpense(
                expense,
                fileName
            );

        if (!success)
        {
            QMessageBox::critical(
                this,
                "Error",
                "Unable to save expense."
            );

            return;
        }

        refreshTable();

        clearForm();

        QMessageBox::information(
            this,
            "Success",
            "Expense added successfully."
        );
    }

    // ==========================================
    // START EDITING
    // ==========================================

    void startEditing()
    {
        int row =
            expenseTable->currentRow();

        if (row < 0)
        {
            QMessageBox::warning(
                this,
                "No Selection",
                "Please select an expense to edit."
            );

            return;
        }

        editingId =
            expenseTable
                ->item(row, 0)
                ->text()
                .toInt();

        Expense* expense =
            manager.getExpenseById(editingId);

        if (expense == nullptr)
        {
            return;
        }

        QDate expenseDate =
            QDate::fromString(
                expense->date,
                "yyyy-MM-dd"
            );

        if (expenseDate.isValid())
        {
            dateEdit->setDate(expenseDate);
        }

        int categoryIndex =
            categoryBox->findText(
                expense->category
            );

        if (categoryIndex >= 0)
        {
            categoryBox->setCurrentIndex(
                categoryIndex
            );
        }

        amountBox->setValue(
            expense->amount
        );

        descriptionEdit->setText(
            expense->description
        );

        addButton->setEnabled(false);
        editButton->setEnabled(false);

        saveButton->setEnabled(true);
        cancelButton->setEnabled(true);
    }

    // ==========================================
    // SAVE EDIT
    // ==========================================

    void saveEdit()
    {
        if (editingId == -1)
        {
            return;
        }

        if (amountBox->value() <= 0)
        {
            QMessageBox::warning(
                this,
                "Invalid Amount",
                "Please enter a valid amount."
            );

            return;
        }

        Expense expense;

        expense.id = editingId;

        expense.date =
            dateEdit->date()
                .toString("yyyy-MM-dd");

        expense.category =
            categoryBox->currentText();

        expense.amount =
            amountBox->value();

        expense.description =
            descriptionEdit->text();

        bool success =
            manager.updateExpense(
                expense,
                fileName
            );

        if (!success)
        {
            QMessageBox::critical(
                this,
                "Error",
                "Unable to update expense."
            );

            return;
        }

        refreshTable();

        editingId = -1;

        addButton->setEnabled(true);
        editButton->setEnabled(true);

        saveButton->setEnabled(false);
        cancelButton->setEnabled(false);

        clearForm();

        QMessageBox::information(
            this,
            "Success",
            "Expense updated successfully."
        );
    }

    // ==========================================
    // CANCEL EDIT
    // ==========================================

    void cancelEdit()
    {
        editingId = -1;

        clearForm();

        addButton->setEnabled(true);
        editButton->setEnabled(true);

        saveButton->setEnabled(false);
        cancelButton->setEnabled(false);
    }

    // ==========================================
    // DELETE
    // ==========================================

    void deleteExpense()
    {
        int row =
            expenseTable->currentRow();

        if (row < 0)
        {
            QMessageBox::warning(
                this,
                "No Selection",
                "Please select an expense to delete."
            );

            return;
        }

        int id =
            expenseTable
                ->item(row, 0)
                ->text()
                .toInt();

        QMessageBox::StandardButton reply =
            QMessageBox::question(
                this,
                "Confirm Delete",
                "Are you sure you want to delete this expense?",
                QMessageBox::Yes |
                QMessageBox::No
            );

        if (reply != QMessageBox::Yes)
        {
            return;
        }

        bool success =
            manager.deleteExpense(
                id,
                fileName
            );

        if (!success)
        {
            QMessageBox::critical(
                this,
                "Error",
                "Unable to delete expense."
            );

            return;
        }

        refreshTable();
    }

    // ==========================================
    // REFRESH TABLE
    // ==========================================

    void refreshTable()
    {
        std::vector<Expense> expenses =
            manager.getExpenses();

        expenseTable->setRowCount(
            static_cast<int>(
                expenses.size()
            )
        );

        for (
            int i = 0;
            i < static_cast<int>(expenses.size());
            ++i
        )
        {
            const Expense& expense =
                expenses[i];

            expenseTable->setItem(
                i,
                0,
                new QTableWidgetItem(
                    QString::number(
                        expense.id
                    )
                )
            );

            expenseTable->setItem(
                i,
                1,
                new QTableWidgetItem(
                    expense.date
                )
            );

            expenseTable->setItem(
                i,
                2,
                new QTableWidgetItem(
                    expense.category
                )
            );

            expenseTable->setItem(
                i,
                3,
                new QTableWidgetItem(
                    QString::number(
                        expense.amount,
                        'f',
                        2
                    )
                )
            );

            expenseTable->setItem(
                i,
                4,
                new QTableWidgetItem(
                    expense.description
                )
            );
        }
    }

    // ==========================================
    // CLEAR FORM
    // ==========================================

    void clearForm()
    {
        dateEdit->setDate(
            QDate::currentDate()
        );

        categoryBox->setCurrentIndex(0);

        amountBox->setValue(0);

        descriptionEdit->clear();
    }
};

// ==========================================
// MAIN
// ==========================================

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ExpenseWindow window;

    window.show();

    return app.exec();
}