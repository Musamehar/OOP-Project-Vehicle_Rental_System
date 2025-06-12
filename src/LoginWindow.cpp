#include "LoginWindow.h"
#include "DatabaseManager.h"
#include "MainWindow.h"
#include "Admin.h"
#include "AdminDashboard.h"
#include "CustomerDashboard.h"
#include "Customer.h"
#include "CustomerFormPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QSettings>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>

LoginWindow::LoginWindow(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadRememberedCredentials();
    setupConnections();
}

void LoginWindow::setupUI() {
    setWindowTitle("Login - Royal Car Rentals");
    resize(1000, 600);
    currentImageIndex = 0;
    startImageSlideshow();

    auto* mainLayout = new QHBoxLayout(this);

    // Left login form panel
    QWidget* leftPanel = new QWidget(this);
    leftPanel->setMinimumWidth(400);
    QVBoxLayout* outerLeftLayout = new QVBoxLayout(leftPanel);
    outerLeftLayout->setContentsMargins(20, 20, 20, 20);

    QWidget* formContainer = new QWidget(leftPanel);
    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setAlignment(Qt::AlignCenter);

    QLabel* appName = new QLabel("Royal Car Rentals");
    appName->setStyleSheet("font-size: 24px; font-weight: bold; color: #1e88e5;");
    appName->setAlignment(Qt::AlignCenter);

    QLabel* welcome = new QLabel("Welcome");
    welcome->setStyleSheet("font-size: 18px; color: white;");
    welcome->setAlignment(Qt::AlignCenter);

    QLabel* emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet("color: white;");
    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("Enter your email");

    QLabel* passLabel = new QLabel("Password:");
    passLabel->setStyleSheet("color: white;");
    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);

    togglePasswordButton = new QPushButton("👁");
    togglePasswordButton->setFixedWidth(30);

    auto* passRow = new QHBoxLayout();
    passRow->addWidget(passwordInput);
    passRow->addWidget(togglePasswordButton);

    rememberCheck = new QCheckBox("Remember Me");

    QPushButton* forgot = new QPushButton("Forgot Password?");
    forgot->setFlat(true);
    forgot->setStyleSheet("color: #1e88e5;");
    forgot->setCursor(Qt::PointingHandCursor);

    loginButton = new QPushButton("Login");

    auto* socialRow = new QHBoxLayout();
    socialRow->addWidget(new QPushButton("Google"));
    socialRow->addWidget(new QPushButton("Facebook"));

    QPushButton* signUp = new QPushButton("Don't have an account? Sign up");
    signUp->setFlat(true);
    signUp->setStyleSheet("color: #1e88e5;");
    signUp->setCursor(Qt::PointingHandCursor);
    signUp->setObjectName("signUpButton");

    formLayout->addWidget(appName);
    formLayout->addWidget(welcome);
    formLayout->addSpacing(10);
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailInput);
    formLayout->addWidget(passLabel);
    formLayout->addLayout(passRow);
    formLayout->addWidget(rememberCheck);
    formLayout->addWidget(forgot);
    formLayout->addWidget(loginButton);
    formLayout->addLayout(socialRow);
    formLayout->addSpacing(10);
    formLayout->addWidget(signUp, 0, Qt::AlignCenter);

    outerLeftLayout->addStretch();
    outerLeftLayout->addWidget(formContainer, 0, Qt::AlignVCenter);
    outerLeftLayout->addStretch();

    // Right image panel
    QWidget* rightPanel = new QWidget(this);
    rightPanel->setMinimumWidth(400);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);  // allow full scaling
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setStyleSheet("background-color: black;");
    rightLayout->addWidget(imageLabel);

    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(rightPanel, 1);

    setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: white;
        }
        QLineEdit {
            padding: 8px;
            background-color: #1e1e1e;
            border: 1px solid #1e88e5;
            color: white;
            border-radius: 4px;
        }
        QPushButton {
            padding: 8px;
            background-color: #1e88e5;
            border: none;
            color: white;
            border-radius: 4px;
        }
        QPushButton:flat {
            background-color: transparent;
        }
        QPushButton:hover {
            background-color: #042236;
        }
    )");

    emailInput->setFocus();
}


void LoginWindow::setupConnections() {
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(togglePasswordButton, &QPushButton::clicked, this, &LoginWindow::togglePasswordVisibility);

    QPushButton* signUpBtn = findChild<QPushButton*>("signUpButton");
    if (signUpBtn)
        connect(signUpBtn, &QPushButton::clicked, this, &LoginWindow::handleSignUp);


    connect(emailInput, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
    connect(passwordInput, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
}

void LoginWindow::loadRememberedCredentials() {
    QSettings settings("RoyalCarRentals", "Login");
    QString savedEmail = settings.value("email").toString();
    QString savedPassword = settings.value("password").toString();
    bool remembered = settings.value("remember", false).toBool();

    if (remembered) {
        emailInput->setText(savedEmail);
        passwordInput->setText(savedPassword);
        rememberCheck->setChecked(true);
    }
}

void LoginWindow::saveCredentialsIfNeeded() {
    QSettings settings("RoyalCarRentals", "Login");
    if (rememberCheck->isChecked()) {
        settings.setValue("email", emailInput->text());
        settings.setValue("password", passwordInput->text());
        settings.setValue("remember", true);
    } else {
        settings.clear();
    }
}

void LoginWindow::togglePasswordVisibility() {
    bool isHidden = passwordInput->echoMode() == QLineEdit::Password;
    passwordInput->setEchoMode(isHidden ? QLineEdit::Normal : QLineEdit::Password);
}

void LoginWindow::handleLogin() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Please enter both email and password.");
        return;
    }

    qDebug() << "🔍 Looking for user with email:" << email;
    QSqlQuery query;
    query.prepare("SELECT name, email, password, contact, role FROM users WHERE email = ?");
    query.addBindValue(email);

    if (!query.exec()) {
        qWarning() << "❌ Query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to query database.");
        return;
    }

    if (!query.next()) {
        qWarning() << "❌ User not found:" << email;
        QMessageBox::warning(this, "Login Failed", "User not found.");
        return;
    }

    QString name     = query.value(0).toString();
    QString dbEmail  = query.value(1).toString();
    QString dbPass   = query.value(2).toString();
    QString contact  = query.value(3).toString();
    QString role     = query.value(4).toString();

    qDebug() << "✅ Found user:" << name << ", role:" << role;

    if (password == dbPass) {
        saveCredentialsIfNeeded();
        this->close();

        if (role == "admin") {
            auto* win = new MainWindow(new Admin(name, dbEmail, dbPass, contact));
            win->show();
        } else if (role == "customer") {
            Customer* customer = new Customer(name, dbEmail, dbPass, contact);
            auto* win = new CustomerDashboard(customer);
            win->show();

            win->show();
        } else {
            QMessageBox::warning(this, "Error", "Unknown user role.");
        }
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect password.");
    }
    int userId   = query.value(0).toInt();
    User* user = nullptr;
    if (role == "admin") {
        auto* admin = new Admin(name, dbEmail, dbPass, contact);
        admin->setId(userId);
        user = admin;
    } else if (role == "customer") {
        auto* cust = new Customer(name, dbEmail, dbPass, contact);
        cust->setId(userId);
        user = cust;
    }

}

void LoginWindow::handleSignUp() {
    CustomerFormPage* form = new CustomerFormPage(true, -1, this);  // Create mode
    form->exec();
}

void LoginWindow::startImageSlideshow() {
    imageList << ":/assets/car1.jpg" << ":/assets/car2.jpg" << ":/assets/car3.jpg" << ":/assets/car4.jpg" << ":/assets/car5.jpg" << ":/assets/car6.jpg";  // ✅ add your images to .qrc
    slideshowTimer = new QTimer(this);
    connect(slideshowTimer, &QTimer::timeout, this, [this]() {
        if (imageList.isEmpty()) return;
        QPixmap pix(imageList[currentImageIndex]);
        imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        currentImageIndex = (currentImageIndex + 1) % imageList.size();
    });
    slideshowTimer->start(3000); // every 3 seconds
}

