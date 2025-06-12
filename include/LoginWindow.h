#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QTimer>

class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

private:
    QLineEdit* emailInput;
    QLineEdit* passwordInput;
    QPushButton* loginButton;
    QPushButton* togglePasswordButton;
    QCheckBox* rememberCheck;
    QLabel* imageLabel;

    QStringList imageList;
    int currentImageIndex;
    QTimer* slideshowTimer;

    void setupUI();
    void setupConnections();
    void loadRememberedCredentials();
    void saveCredentialsIfNeeded();
    void startImageSlideshow();

private slots:
    void handleLogin();
    void handleSignUp();
    void togglePasswordVisibility();
};

#endif // LOGINWINDOW_H
