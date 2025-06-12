#ifndef INVOICEPAGE_H
#define INVOICEPAGE_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class InvoicePage : public QWidget {
    Q_OBJECT

    QTextEdit* invoiceText;
    QPushButton* printButton;
    QPushButton* exportPdfButton;
    int bookingId;

    void loadInvoice();

public:
    InvoicePage(int bookingId, QWidget* parent = nullptr);

private slots:
    void printInvoice();
    void exportToPDF();
};

#endif
