#include "InvoicePage.h"
#include "DatabaseManager.h"
#include <QPrintDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>


InvoicePage::InvoicePage(int bookingId, QWidget* parent)
    : QWidget(parent), bookingId(bookingId) {
    invoiceText = new QTextEdit(this);
    invoiceText->setReadOnly(true);
    printButton = new QPushButton("Print Invoice", this);
    exportPdfButton = new QPushButton("Export PDF", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(invoiceText);
    layout->addWidget(printButton);
    layout->addWidget(exportPdfButton);

    connect(printButton, &QPushButton::clicked, this, &InvoicePage::printInvoice);
    connect(exportPdfButton, &QPushButton::clicked, this, &InvoicePage::exportToPDF);

    loadInvoice();
}

void InvoicePage::loadInvoice() {
    QSqlQuery query = DatabaseManager::getInvoiceDetails(bookingId);
    if (!query.next()) {
        invoiceText->setText("Error loading invoice data.");
        return;
    }

    QString customer = query.value("customer").toString();
    QString vehicle = query.value("vehicle").toString();
    QString startDate = query.value("start_date").toString();
    QString endDate = query.value("end_date").toString();
    QString returnDate = query.value("return_date").toString();
    double amount = query.value("total_amount").toDouble();
    double penalty = query.value("penalty_fee").toDouble();

    double total = amount + penalty;

    QString html = QString(R"(
        <h2>Royal Car Rentals</h2>
        <h3>Invoice</h3>
        <p><b>Customer:</b> %1</p>
        <p><b>Vehicle:</b> %2</p>
        <p><b>Rental Period:</b> %3 → %4</p>
        <p><b>Returned:</b> %5</p>
        <hr>
        <p><b>Rental Amount:</b> $%6</p>
        <p><b>Penalty:</b> $%7</p>
        <p><b><u>Total:</u></b> $%8</p>
    )").arg(customer, vehicle, startDate, endDate, returnDate)
                       .arg(amount, 0, 'f', 2)
                       .arg(penalty, 0, 'f', 2)
                       .arg(total, 0, 'f', 2);

    invoiceText->setHtml(html);
}

void InvoicePage::printInvoice() {
    QTextDocument doc;
    doc.setHtml(invoiceText->toHtml());
    QPrinter printer;
    printer.setPrinterName("Printer");
    if (QPrintDialog(&printer).exec() == QDialog::Accepted)
        doc.print(&printer);
}

void InvoicePage::exportToPDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export Invoice", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QTextDocument doc;
    doc.setHtml(invoiceText->toHtml());

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    doc.print(&printer);

    QMessageBox::information(this, "Exported", "Invoice exported to PDF successfully.");
}
