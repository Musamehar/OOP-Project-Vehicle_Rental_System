#ifndef CUSTOMERFORMPAGE_H
#define CUSTOMERFORMPAGE_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QFormLayout;

class CustomerFormPage : public QDialog {
    Q_OBJECT

    QLineEdit *nameInput, *emailInput, *passwordInput, *contactInput;
    QPushButton *saveButton;
    bool isEditMode;
    int customerId;

    void setupForm();
    void loadCustomerDetails();

public:
    CustomerFormPage(bool editMode = false, int customerId = -1, QWidget* parent = nullptr);

private slots:
    void saveOrUpdateCustomer();
};

#endif
