#include "CustomerDashboard.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>
#include <QFont>
#include <QComboBox>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGroupBox>
#include <QPixmap>
#include <QFileInfo>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QBuffer>
#include <QSpacerItem>
#include <QSizePolicy>

CustomerDashboard::CustomerDashboard(Customer* customer, QWidget* loginWindow)
    : currentCustomer(customer), loginRef(loginWindow)
{
    setWindowTitle("Royal Car Rentals - Customer Dashboard");
    setMinimumSize(1200, 800);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Header with Royal branding
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* logoLabel = new QLabel("ROYAL");
    logoLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1e88e5; padding: 10px;");
    headerLayout->addWidget(logoLabel);
    headerLayout->addStretch();

    // Navigation bar
    QHBoxLayout* navbar = new QHBoxLayout();

    QPushButton* homeBtn = new QPushButton("Home");
    QPushButton* bookingBtn = new QPushButton("Booking");
    QPushButton* aboutBtn = new QPushButton("About Us");
    QPushButton* termsBtn = new QPushButton("Terms & Conditions");
    QPushButton* settingsBtn = new QPushButton("⚙ Settings");

    QList<QPushButton*> navButtons = { homeBtn, bookingBtn, aboutBtn, termsBtn, settingsBtn };
    for (auto* btn : navButtons) {
        styleNavbar(btn);
        navbar->addWidget(btn);
    }
    navbar->addStretch();

    // Content stack
    contentStack = new QStackedWidget();
    contentStack->addWidget(createHomePage());       // Index 0 - Home page
    contentStack->addWidget(createBookingPage());    // Index 1
    contentStack->addWidget(createAboutPage());      // Index 2
    contentStack->addWidget(createTermsPage());      // Index 3
    contentStack->addWidget(createSettingsPage());   // Index 4

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(navbar);
    mainLayout->addWidget(contentStack);
    setLayout(mainLayout);

    setStyleSheet("background-color: #0D0D0D; color: white;");

    // Connect signals
    connect(homeBtn, &QPushButton::clicked, this, &CustomerDashboard::showHomePage);
    connect(bookingBtn, &QPushButton::clicked, this, &CustomerDashboard::showBookingPage);
    connect(aboutBtn, &QPushButton::clicked, this, &CustomerDashboard::showAboutPage);
    connect(termsBtn, &QPushButton::clicked, this, &CustomerDashboard::showTermsPage);
    connect(settingsBtn, &QPushButton::clicked, this, &CustomerDashboard::showSettingsPage);

    // Load car data
    loadCarsFromDatabase();

    // Show home page by default
    contentStack->setCurrentIndex(0);
}
QWidget* CustomerDashboard::createHomePage()
{
    QWidget* page = new QWidget;
    page->setObjectName("HomePage");

    // Set background image on the page (adjust path accordingly)
    page->setStyleSheet(R"(
        QWidget#HomePage {
            background-image: url('C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/bg1.jpg'); /* Replace with your image path */
            background-repeat: no-repeat;
            background-position: center;
            background-size: cover;
        }
    )");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    // Transparent container for text (no background, no border)
    QWidget* textContainer = new QWidget(page);
    textContainer->setAttribute(Qt::WA_TranslucentBackground); // Make sure container is transparent
    textContainer->setStyleSheet("background: transparent; border: none;");

    QVBoxLayout* textLayout = new QVBoxLayout(textContainer);
    textLayout->setAlignment(Qt::AlignCenter);

    // Title label
    QLabel* title = new QLabel("Welcome to Royal Car Rental");
    title->setStyleSheet("font-size: 38px; font-weight: bold; color: white;");
    title->setAlignment(Qt::AlignCenter);

    // Subtitle label
    QLabel* subtitle = new QLabel("Manage your fleet and rentals with ease");
    subtitle->setStyleSheet("font-size: 18px; font-weight: 600; color: white;");
    subtitle->setAlignment(Qt::AlignCenter);

    textLayout->addWidget(title);
    textLayout->addWidget(subtitle);

    textContainer->setFixedSize(700, 250);

    layout->addWidget(textContainer, 0, Qt::AlignCenter);

    return page;
}


void CustomerDashboard::showHomePage() {
    contentStack->setCurrentIndex(0); // Home page is at index 0
}


void CustomerDashboard::styleNavbar(QPushButton* btn)
{
    btn->setStyleSheet(R"(
        QPushButton {
            color: white;
            background-color: #1e88e5;
            border: none;
            padding: 12px 24px;
            border-radius: 6px;
            font-size: 14px;
            margin: 0 5px;
        }
        QPushButton:hover {
            background-color: #0d47a1;
        }
        QPushButton:pressed {
            background-color: #0d47a1;
        }
    )");
}

QWidget* CustomerDashboard::createBookingPage()
{
    QWidget* page = new QWidget;
    page->setStyleSheet("background-color: #181f26; color: #fff; font-family: 'Segoe UI', Arial, sans-serif;");

    QHBoxLayout* mainLayout = new QHBoxLayout(page);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left sidebar with filters
    QWidget* leftSidebar = createFilterSection();
    leftSidebar->setMaximumWidth(300);
    leftSidebar->setMinimumWidth(260);

    // Right content area
    QWidget* rightContent = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContent);
    rightLayout->setContentsMargins(32, 24, 32, 24);
    rightLayout->setSpacing(24);

    // Search section
    QWidget* searchSection = createSearchSection();
    rightLayout->addWidget(searchSection);

    // Car display area
    QScrollArea* carScroll = new QScrollArea;
    carScroll->setWidgetResizable(true);
    carScroll->setFrameShape(QFrame::NoFrame);
    carScroll->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical {
            background: #232e3b;
            width: 12px;
            margin: 8px 2px 8px 2px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background: #1e88e5;
            min-height: 30px;
            border-radius: 6px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
    )");

    carContainer = new QWidget;
    carScroll->setWidget(carContainer);

    rightLayout->addWidget(carScroll);

    mainLayout->addWidget(leftSidebar);
    mainLayout->addWidget(rightContent);

    // Display initial cars
    displayCars(allCars);

    return page;
}

QWidget* CustomerDashboard::createFilterSection()
{
    QWidget* filterWidget = new QWidget;
    filterWidget->setStyleSheet(R"(
        background-color: #1a232a;
        border-top-right-radius: 24px;
        border-bottom-right-radius: 24px;
        border-right: 2px solid #222;
        box-shadow: 2px 0 16px #0004;
    )");
    QVBoxLayout* layout = new QVBoxLayout(filterWidget);
    layout->setContentsMargins(24, 32, 24, 32);
    layout->setSpacing(18);

    QLabel* filterTitle = new QLabel("🔎 Filters");
    filterTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #1e88e5; margin-bottom: 18px;");
    layout->addWidget(filterTitle);

    // Car Type Filter
    QGroupBox* carTypeGroup = new QGroupBox("Car Type");
    carTypeGroup->setStyleSheet(R"(
        QGroupBox {
            background: #232e3b;
            border-radius: 14px;
            margin-top: 12px;
        }
        QGroupBox::title {
            color: #1e88e5;
            font-weight: bold;
            font-size: 16px;
            margin-left: 8px;
        }
    )");
    QVBoxLayout* carTypeLayout = new QVBoxLayout(carTypeGroup);

    this->carTypeGroup = new QButtonGroup(this);
    QCheckBox* coupeCheck = new QCheckBox("Coupe (24)");
    QCheckBox* hatchbackCheck = new QCheckBox("Hatchback (12)");
    QCheckBox* sedanCheck = new QCheckBox("Sedan (16)");
    QCheckBox* mpvCheck = new QCheckBox("MPV (28)");
    QCheckBox* suvCheck = new QCheckBox("SUV (20)");

    for (auto cb : {coupeCheck, hatchbackCheck, sedanCheck, mpvCheck, suvCheck}) {
        cb->setStyleSheet(R"(
            QCheckBox {
                color: #fff;
                font-size: 15px;
                padding: 6px 0;
            }
            QCheckBox::indicator {
                width: 20px; height: 20px;
            }
            QCheckBox::indicator:unchecked {
                background: #232e3b;
                border: 2px solid #1e88e5;
                border-radius: 4px;
            }
            QCheckBox::indicator:checked {
                background: #1e88e5;
                border: 2px solid #1e88e5;
                border-radius: 4px;
            }
        )");
        carTypeLayout->addWidget(cb);
    }

    // Capacity Filter
    QGroupBox* capacityGroup = new QGroupBox("Capacity");
    capacityGroup->setStyleSheet(carTypeGroup->styleSheet());
    QVBoxLayout* capacityLayout = new QVBoxLayout(capacityGroup);

    this->capacityGroup = new QButtonGroup(this);
    QCheckBox* twoFiveCheck = new QCheckBox("2-5 (100)");
    QCheckBox* sixMoreCheck = new QCheckBox("6 or more (4)");
    for (auto cb : {twoFiveCheck, sixMoreCheck}) {
        cb->setStyleSheet(coupeCheck->styleSheet());
        capacityLayout->addWidget(cb);
    }

    // Customer Recommendation Filter
    QGroupBox* recommendationGroup = new QGroupBox("Customer Recommendation");
    recommendationGroup->setStyleSheet(carTypeGroup->styleSheet());
    QVBoxLayout* recommendationLayout = new QVBoxLayout(recommendationGroup);

    this->recommendationGroup = new QButtonGroup(this);
    QCheckBox* seventyUpCheck = new QCheckBox("70% & up (72)");
    QCheckBox* fortyUpCheck = new QCheckBox("40% & up (28)");
    for (auto cb : {seventyUpCheck, fortyUpCheck}) {
        cb->setStyleSheet(coupeCheck->styleSheet());
        recommendationLayout->addWidget(cb);
    }

    layout->addWidget(carTypeGroup);
    layout->addWidget(capacityGroup);
    layout->addWidget(recommendationGroup);
    layout->addStretch();

    // Connect filter signals
    connect(coupeCheck, &QCheckBox::toggled, this, &CustomerDashboard::applyFilters);
    connect(hatchbackCheck, &QCheckBox::toggled, this, &CustomerDashboard::applyFilters);
    connect(sedanCheck, &QCheckBox::toggled, this, &CustomerDashboard::applyFilters);
    connect(mpvCheck, &QCheckBox::toggled, this, &CustomerDashboard::applyFilters);
    connect(suvCheck, &QCheckBox::toggled, this, &CustomerDashboard::applyFilters);

    return filterWidget;
}

QWidget* CustomerDashboard::createSearchSection()
{
    QWidget* searchWidget = new QWidget;
    searchWidget->setStyleSheet(R"(
        background: #232e3b;
        border-radius: 16px;
        box-shadow: 0 2px 16px #0002;
    )");
    QVBoxLayout* layout = new QVBoxLayout(searchWidget);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(10);

    QLabel* banner = new QLabel("🚗 Book Your Car in Easy Steps");
    banner->setStyleSheet("font-size: 30px; font-weight: bold; color: #1e88e5; margin-bottom: 8px;");
    layout->addWidget(banner);

    QLabel* subtitle = new QLabel("Find the perfect car at the best price. Enjoy freedom and flexibility on your journey!");
    subtitle->setStyleSheet("color: #bbb; font-size: 15px; margin-bottom: 18px;");
    layout->addWidget(subtitle);

    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->setSpacing(18);

    auto comboStyle = "padding: 8px 12px; background-color: #181f26; border: 1.5px solid #1e88e5; color: white; border-radius: 8px; font-size: 15px;";

    QVBoxLayout* pickupLayout = new QVBoxLayout;
    QLabel* pickupLabel = new QLabel("🌍 Pick-up");
    pickupLabel->setStyleSheet("color: #1e88e5; font-weight: bold; font-size: 14px;");
    pickupLocation = new QComboBox;
    pickupLocation->addItems({"London (LHR - Heathrow)", "London (LGW - Gatwick)", "Manchester", "Birmingham"});
    pickupLocation->setStyleSheet(comboStyle);
    pickupLayout->addWidget(pickupLabel);
    pickupLayout->addWidget(pickupLocation);

    QVBoxLayout* dropoffLayout = new QVBoxLayout;
    QLabel* dropoffLabel = new QLabel("🌍 Drop-off");
    dropoffLabel->setStyleSheet("color: #1e88e5; font-weight: bold; font-size: 14px;");
    dropoffLocation = new QComboBox;
    dropoffLocation->addItems({"London (LGW - Gatwick)", "London (LHR - Heathrow)", "Manchester", "Birmingham"});
    dropoffLocation->setStyleSheet(comboStyle);
    dropoffLayout->addWidget(dropoffLabel);
    dropoffLayout->addWidget(dropoffLocation);

    auto dateStyle = "padding: 8px 12px; background-color: #181f26; border: 1.5px solid #1e88e5; color: white; border-radius: 8px; font-size: 15px;";

    QVBoxLayout* pickupDateLayout = new QVBoxLayout;
    QLabel* pickupDateLabel = new QLabel("📅 Pick-up Date");
    pickupDateLabel->setStyleSheet("color: #1e88e5; font-weight: bold; font-size: 14px;");
    pickupDate = new QDateEdit(QDate::currentDate());
    pickupDate->setStyleSheet(dateStyle);
    pickupDateLayout->addWidget(pickupDateLabel);
    pickupDateLayout->addWidget(pickupDate);

    QVBoxLayout* dropoffDateLayout = new QVBoxLayout;
    QLabel* dropoffDateLabel = new QLabel("📅 Drop-off Date");
    dropoffDateLabel->setStyleSheet("color: #1e88e5; font-weight: bold; font-size: 14px;");
    dropoffDate = new QDateEdit(QDate::currentDate().addDays(1));
    dropoffDate->setStyleSheet(dateStyle);
    dropoffDateLayout->addWidget(dropoffDateLabel);
    dropoffDateLayout->addWidget(dropoffDate);

    QVBoxLayout* pickupTimeLayout = new QVBoxLayout;
    QLabel* pickupTimeLabel = new QLabel("🕐 Pick-up Time");
    pickupTimeLabel->setStyleSheet("color: #1e88e5; font-weight: bold; font-size: 14px;");
    pickupTime = new QTimeEdit(QTime(10, 0));
    pickupTime->setStyleSheet(dateStyle);
    pickupTimeLayout->addWidget(pickupTimeLabel);
    pickupTimeLayout->addWidget(pickupTime);

    QVBoxLayout* dropoffTimeLayout = new QVBoxLayout;
    QLabel* dropoffTimeLabel = new QLabel("🕐 Drop-off Time");
    dropoffTimeLabel->setStyleSheet("color: #1e88e5; font-weight: bold; font-size: 14px;");
    dropoffTime = new QTimeEdit(QTime(10, 0));
    dropoffTime->setStyleSheet(dateStyle);
    dropoffTimeLayout->addWidget(dropoffTimeLabel);
    dropoffTimeLayout->addWidget(dropoffTime);

    searchLayout->addLayout(pickupLayout);
    searchLayout->addLayout(dropoffLayout);
    searchLayout->addLayout(pickupDateLayout);
    searchLayout->addLayout(dropoffDateLayout);
    searchLayout->addLayout(pickupTimeLayout);
    searchLayout->addLayout(dropoffTimeLayout);

    QPushButton* searchBtn = new QPushButton("🔍 Search ");
    searchBtn->setFixedHeight(44);
    searchBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #1e88e5;
            color: white;
            font-size: 17px;
            font-weight: bold;
            border-radius: 10px;
            padding: 0 28px;
        }
        QPushButton:hover {
            background-color: #1565c0;
        }
    )");
    searchLayout->addWidget(searchBtn);

    layout->addLayout(searchLayout);

    return searchWidget;
}

QWidget* CustomerDashboard::createCarCard(const CarData& car)
{
    QWidget* card = new QWidget;
    card->setFixedSize(370, 430);

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(340, 180);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border-radius: 14px; background: #232e3b; box-shadow: 0 2px 12px #0002;");
    if (!car.image.isEmpty()) {
        QPixmap pix(car.image);
        if (!pix.isNull()) {
            imageLabel->setPixmap(pix.scaled(340, 180, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        } else {
            imageLabel->setText("Image not found");
            imageLabel->setStyleSheet("color: #888; font-size: 18px;");
        }
    } else {
        imageLabel->setText("No Image");
        imageLabel->setStyleSheet("color: #888; font-size: 18px;");
    }
    layout->addWidget(imageLabel);

    QHBoxLayout* nameRow = new QHBoxLayout;
    QLabel* nameLabel = new QLabel(car.name);
    nameLabel->setStyleSheet("font-size: 19px; font-weight: bold; color: #fff;");
    nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QLabel* badge = new QLabel("⭐ " + QString::number(car.rating, 'f', 1));
    badge->setStyleSheet("background: #1e88e5; color: white; font-size: 14px; border-radius: 8px; padding: 2px 10px; font-weight: 600;");
    badge->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    nameRow->addWidget(nameLabel);
    nameRow->addStretch();
    nameRow->addWidget(badge);
    layout->addLayout(nameRow);

    QHBoxLayout* detailsRow = new QHBoxLayout;
    QLabel* typeIcon = new QLabel("🚗");
    typeIcon->setStyleSheet("font-size: 16px;");
    QLabel* typeLabel = new QLabel(car.type);
    typeLabel->setStyleSheet("color: #bbb; font-size: 14px;");
    QLabel* capIcon = new QLabel("👥");
    capIcon->setStyleSheet("font-size: 16px;");
    QLabel* capLabel = new QLabel(QString("%1 seats").arg(car.capacity));
    capLabel->setStyleSheet("color: #bbb; font-size: 14px;");
    detailsRow->addWidget(typeIcon);
    detailsRow->addWidget(typeLabel);
    detailsRow->addSpacing(16);
    detailsRow->addWidget(capIcon);
    detailsRow->addWidget(capLabel);
    detailsRow->addStretch();
    layout->addLayout(detailsRow);

    layout->addSpacing(8);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    QPushButton* rentBtn = new QPushButton("Rent Now");
    rentBtn->setFixedWidth(130);
    rentBtn->setFixedHeight(70);
    rentBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #43a047;
            color: white;
            font-size: 16px;
            font-weight: bold;
            border-radius: 8px;
            padding: 12px 0;
        }
        QPushButton:hover {
            background-color: #388e3c;
        }
    )");
    QLabel* priceLabel = new QLabel(QString("$%1/day").arg(car.pricePerDay, 0, 'f', 0));
    priceLabel->setAlignment(Qt::AlignCenter);
    priceLabel->setStyleSheet("font-size: 20px; color: #1e88e5; font-weight: bold; margin-left: 14px;");
    bottomLayout->addWidget(rentBtn);
    bottomLayout->addSpacing(18);
    bottomLayout->addWidget(priceLabel);
    bottomLayout->addStretch();

    layout->addLayout(bottomLayout);

    connect(rentBtn, &QPushButton::clicked, this, [this, car]() {
        showRentDialog(car);
    });

    card->setStyleSheet(R"(
        QWidget {
            background-color: #232e3b;
            color: #fff;
            border: 2px solid #232e3b;
            border-radius: 18px;
            box-shadow: 0 4px 28px #0004;
            padding: 16px;
            margin: 12px;
            transition: border 0.2s;
        }
        QWidget:hover {
            border: 2px solid #1e88e5;
        }
    )");

    return card;
}


void CustomerDashboard::showRentDialog(const CarData& car)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Confirm Rental");
    dialog.setModal(true);
    dialog.setFixedSize(400, 480);
    dialog.setStyleSheet("background:#232323; color:white;");

    QVBoxLayout* vbox = new QVBoxLayout(&dialog);

    QLabel* title = new QLabel("Confirm Your Booking");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #1e88e5;");
    vbox->addWidget(title, 0, Qt::AlignCenter);

    // Car image
    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(220, 100);
    imageLabel->setAlignment(Qt::AlignCenter);
    if (!car.image.isEmpty() && QFileInfo::exists(car.image)) {
        QPixmap pix(car.image);
        imageLabel->setPixmap(pix.scaled(220, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        imageLabel->setText("No Image");
        imageLabel->setStyleSheet("color: #888; font-size: 14px;");
    }
    vbox->addWidget(imageLabel, 0, Qt::AlignCenter);

    QLabel* nameLabel = new QLabel(car.name);
    nameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white;");
    nameLabel->setAlignment(Qt::AlignCenter);
    vbox->addWidget(nameLabel);

    QLabel* priceLabel = new QLabel(QString("Rent: $%1 per day").arg(car.pricePerDay, 0, 'f', 0));
    priceLabel->setStyleSheet("font-size: 16px; color: #1e88e5;");
    priceLabel->setAlignment(Qt::AlignCenter);
    vbox->addWidget(priceLabel);

    vbox->addSpacing(10);

    // Mini form for confirmation
    QFormLayout* form = new QFormLayout;
    QLineEdit* nameEdit = new QLineEdit(currentCustomer->getName());
    QLineEdit* contactEdit = new QLineEdit(currentCustomer->getContact());
    form->addRow("Your Name:", nameEdit);
    form->addRow("Contact:", contactEdit);

    // Optionally, show pick-up/drop-off info
    QLabel* infoLabel = new QLabel(QString("Pick-up: %1\nDrop-off: %2")
                                       .arg(pickupLocation->currentText())
                                       .arg(dropoffLocation->currentText()));
    infoLabel->setStyleSheet("color: #bbb; font-size: 12px;");
    vbox->addWidget(infoLabel);

    vbox->addLayout(form);

    // --- Credit Card Section ---
    QLabel* payTitle = new QLabel("Payment (Credit Card)");
    payTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #1e88e5; margin-top: 8px;");
    vbox->addWidget(payTitle);

    QFormLayout* cardForm = new QFormLayout;
    QLineEdit* cardNumber = new QLineEdit;
    cardNumber->setPlaceholderText("Card Number");
    QLineEdit* cardName = new QLineEdit;
    cardName->setPlaceholderText("Name on Card");
    QLineEdit* expiry = new QLineEdit;
    expiry->setPlaceholderText("MM/YY");
    QLineEdit* cvv = new QLineEdit;
    cvv->setPlaceholderText("CVV");
    cvv->setEchoMode(QLineEdit::Password);

    cardForm->addRow("Card Number:", cardNumber);
    cardForm->addRow("Name on Card:", cardName);
    cardForm->addRow("Expiry:", expiry);
    cardForm->addRow("CVV:", cvv);

    vbox->addLayout(cardForm);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    vbox->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        // Validate card fields
        if (cardNumber->text().isEmpty() || cardName->text().isEmpty() || expiry->text().isEmpty() || cvv->text().isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "Please fill in all credit card fields.");
            return;
        }
        // Here you would add booking logic, e.g. call DatabaseManager::addBooking(...)
        QMessageBox::information(this, "Success", "Your booking has been confirmed!\n\nNow you can save your invoice.");

        // --- Invoice PDF Generation ---
        QString fileName = QFileDialog::getSaveFileName(&dialog, "Save Invoice", "invoice.pdf", "PDF Files (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter writer(fileName);
            writer.setPageSize(QPageSize(QPageSize::A4));
            QPainter painter(&writer);

            painter.setFont(QFont("Arial", 16, QFont::Bold));
            painter.drawText(100, 100, "Royal Car Rentals - Invoice");

            painter.setFont(QFont("Arial", 12));
            painter.drawText(100, 180, QString("Customer: %1").arg(nameEdit->text()));
            painter.drawText(100, 210, QString("Contact: %1").arg(contactEdit->text()));
            painter.drawText(100, 240, QString("Car: %1").arg(car.name));
            painter.drawText(100, 270, QString("Price per Day: $%1").arg(car.pricePerDay));
            painter.drawText(100, 300, QString("Pick-up: %1").arg(pickupLocation->currentText()));
            painter.drawText(100, 330, QString("Drop-off: %1").arg(dropoffLocation->currentText()));
            painter.drawText(100, 360, QString("Payment Method: Credit Card (****%1)").arg(cardNumber->text().right(4)));
            painter.drawText(100, 420, "Thank you for booking with Royal Car Rentals!");

            painter.end();

            QMessageBox::information(&dialog, "Invoice Saved", "Invoice PDF has been saved.");
        }

        dialog.accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    dialog.exec();
}


QWidget* CustomerDashboard::createAboutPage()
{
    QWidget* page = new QWidget;
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* content = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(content);

    // Decorative Header
    QLabel* title = new QLabel("About Royal Car Rentals");
    title->setStyleSheet("font-size: 32px; font-weight: bold; color: #1e88e5; margin-top: 30px; margin-bottom: 10px;");
    title->setAlignment(Qt::AlignCenter);

    // Decorative Line
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #1e88e5; background: #1e88e5; height: 2px;");

    // Main About Text (with rich formatting)
    QString aboutText = R"(
        <div style='font-size:17px; color:#f5f5f5;'>
        <p>
        <b>At Royal Car Rentals, we are dedicated to redefining the experience of luxury and prestige in the world of automobiles.</b>
        Our mission is to provide discerning individuals and businesses with unparalleled access to an exclusive fleet of the finest luxury cars and the highest standards of service.
        </p>
        <h3 style='color:#1e88e5;'>Our Story</h3>
        <p>
        Founded in 2020, Royal Car Rentals began with a simple vision: to transform each journey into an unforgettable experience.
        What started as a small collection of premium vehicles has grown into one of the most trusted names in luxury car rentals, serving thousands of satisfied customers across multiple locations.
        </p>
        <h3 style='color:#1e88e5;'>Our Fleet</h3>
        <ul>
        <li>Latest model luxury sedans and coupes</li>
        <li>High-performance sports cars</li>
        <li>Spacious SUVs and premium hatchbacks</li>
        <li>Exotic and rare automobile collections</li>
        <li>Eco-friendly hybrid and electric vehicles</li>
        </ul>
        <h3 style='color:#1e88e5;'>Our Commitment</h3>
        <ul>
        <li>🚗 <b>Premium Quality:</b> Every vehicle in our fleet is meticulously maintained and regularly inspected to ensure the highest standards of safety and performance.</li>
        <li>🌟 <b>Exceptional Service:</b> Our team of automotive experts is dedicated to providing personalized service that exceeds your expectations.</li>
        <li>🔒 <b>Trust & Reliability:</b> With comprehensive insurance coverage and 24/7 roadside assistance, we ensure your peace of mind throughout your rental experience.</li>
        <li>💎 <b>Luxury Experience:</b> From the moment you book until you return your vehicle, we strive to deliver a seamless, luxurious experience that reflects our royal standard.</li>
        </ul>
        <h3 style='color:#1e88e5;'>Why Choose Royal Car Rentals?</h3>
        <ul>
        <li>✓ Extensive fleet of premium and luxury vehicles</li>
        <li>✓ Competitive pricing with transparent fees</li>
        <li>✓ Flexible rental terms and conditions</li>
        <li>✓ Professional customer support available 24/7</li>
        <li>✓ Easy online booking and management system</li>
        <li>✓ Comprehensive insurance options</li>
        <li>✓ Multiple convenient pickup and drop-off locations</li>
        </ul>
        <h3 style='color:#1e88e5;'>Our Vision</h3>
        <p>
        We envision a future where luxury car rental is not just about transportation, but about creating memorable experiences that enhance your journey.
        Whether you're planning a special occasion, a business trip, or simply want to experience the thrill of driving a premium vehicle, Royal Car Rentals is your gateway to automotive excellence.
        </p>
        <h3 style='color:#1e88e5;'>Statistics</h3>
        <ul>
        <li>📈 <b>2020</b>: Year Founded</li>
        <li>👥 <b>50,000+</b>: Happy Customers Served</li>
        <li>🚗 <b>250+</b>: Premium Cars Available</li>
        <li>⭐ <b>4.8/5</b>: Average Customer Rating</li>
        <li>🌍 <b>15</b>: Service Locations</li>
        </ul>
        <p>
        <b>Join the Royal family and experience the difference that true luxury and exceptional service can make. Your journey awaits.</b>
        </p>
        </div>
    )";

    QLabel* aboutLabel = new QLabel(aboutText);
    aboutLabel->setStyleSheet(
        "background: rgba(30,136,229,0.10);"
        "border-radius: 18px;"
        "padding: 30px;"
        "margin-top: 16px;"
        "color: white;"
        "font-size: 16px;"
        );
    aboutLabel->setWordWrap(true);
    aboutLabel->setAlignment(Qt::AlignLeft);
    aboutLabel->setTextFormat(Qt::RichText);

    layout->addWidget(title);
    layout->addWidget(line);
    layout->addWidget(aboutLabel);

    // Optionally, add a team section or images here for more visual appeal

    content->setStyleSheet("background: transparent;");
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; background-color: #0D0D0D; }"
        "QWidget { background: transparent; }"
        );

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(scrollArea);

    page->setStyleSheet("background: #0D0D0D;");
    return page;
}

QWidget* CustomerDashboard::createTermsPage()
{
    QWidget* page = new QWidget;
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* content = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(content);

    // Decorative Header
    QLabel* title = new QLabel("Terms & Conditions");
    title->setStyleSheet("font-size: 32px; font-weight: bold; color: #1e88e5; margin-top: 30px; margin-bottom: 10px;");
    title->setAlignment(Qt::AlignCenter);

    // Decorative Line
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #1e88e5; background: #1e88e5; height: 2px;");

    // Main Terms Text (with rich formatting)
    QString termsText = R"(
        <div style='font-size:15px; color:#f5f5f5;'>
        <p><b>Effective Date:</b> January 1, 2025<br>
        <b>Last Updated:</b> January 1, 2025</p>
        <h3 style='color:#1e88e5;'>1. GENERAL PROVISIONS</h3>
        <b>1.1 Agreement Scope</b>
        <p>These Terms and Conditions govern the rental of vehicles from Royal Car Rentals ("Company", "we", "us") to customers ("Renter", "you"). By renting a vehicle, you agree to be bound by these terms.</p>
        <b>1.2 Legal Requirements</b>
        <p>All renters must be at least 21 years old with a valid driver's license held for minimum 2 years. International customers must present valid passport and international driving permit.</p>
        <h3 style='color:#1e88e5;'>2. RESERVATIONS AND BOOKING</h3>
        <b>2.1 Reservation Process</b>
        <p>Reservations can be made online, by phone, or in person. All reservations require a valid credit card and are subject to vehicle availability.</p>
        <b>2.2 Confirmation</b>
        <p>Reservations are confirmed upon payment of required deposit. Reservation changes must be made at least 24 hours before pickup time.</p>
        <b>2.3 Cancellation Policy</b>
        <ul>
        <li>Free cancellation up to 48 hours before pickup</li>
        <li>50% charge for cancellations 24-48 hours before pickup</li>
        <li>Full charge for cancellations less than 24 hours before pickup</li>
        </ul>
        <h3 style='color:#1e88e5;'>3. RENTAL RATES AND CHARGES</h3>
        <b>3.1 Base Rental Rate</b>
        <p>Rates are quoted per day and include basic insurance coverage. Additional services and equipment are charged separately.</p>
        <b>3.2 Additional Charges</b>
        <ul>
        <li>Late return: $50 per hour</li>
        <li>Excessive mileage: $0.50 per mile over 200 miles/day</li>
        <li>Fuel service: $15 + fuel cost if not returned with same fuel level</li>
        <li>Cleaning fee: $75 for excessively dirty vehicles</li>
        <li>Smoking fee: $200</li>
        <li>Pet cleaning fee: $100</li>
        </ul>
        <b>3.3 Security Deposit</b>
        <p>A security deposit of $500-$2000 (depending on vehicle class) will be authorized on your credit card and released upon satisfactory return.</p>
        <h3 style='color:#1e88e5;'>4. VEHICLE PICKUP AND RETURN</h3>
        <b>4.1 Pickup Requirements</b>
        <ul>
        <li>Valid driver's license</li>
        <li>Credit card in renter's name</li>
        <li>Proof of insurance (if declining ours)</li>
        <li>Age verification</li>
        </ul>
        <b>4.2 Vehicle Inspection</b>
        <p>Renter must inspect vehicle and report any damage before departure. Failure to report pre-existing damage may result in charges.</p>
        <b>4.3 Return Conditions</b>
        <ul>
        <li>At agreed time and location</li>
        <li>With same fuel level as pickup</li>
        <li>In same condition except normal wear</li>
        <li>With all accessories and documentation</li>
        </ul>
        <h3 style='color:#1e88e5;'>5. DRIVER REQUIREMENTS</h3>
        <b>5.1 Authorized Drivers</b>
        <p>Only the renter and additional drivers listed on the agreement may operate the vehicle. Additional drivers must meet same requirements as primary renter.</p>
        <b>5.2 Prohibited Use</b>
        <ul>
        <li>Racing or speed testing</li>
        <li>Towing or pushing</li>
        <li>Off-road driving</li>
        <li>Illegal activities</li>
        <li>Transporting hazardous materials</li>
        <li>Commercial use (unless specifically authorized)</li>
        </ul>
        <h3 style='color:#1e88e5;'>6. INSURANCE AND LIABILITY</h3>
        <b>6.1 Company Insurance</b>
        <p>Basic coverage is included in rental rate covering liability and collision damage with deductible.</p>
        <b>6.2 Renter's Responsibility</b>
        <ul>
        <li>All traffic violations and parking tickets</li>
        <li>Damage to vehicle interior</li>
        <li>Loss or damage to personal property</li>
        <li>Any damage not covered by insurance</li>
        </ul>
        <b>6.3 Optional Coverage</b>
        <ul>
        <li>Collision Damage Waiver (CDW)</li>
        <li>Personal Accident Insurance (PAI)</li>
        <li>Personal Effects Coverage (PEC)</li>
        <li>Supplemental Liability Insurance (SLI)</li>
        </ul>
        <h3 style='color:#1e88e5;'>7. PROHIBITED ACTIVITIES</h3>
        <ul>
        <li>Driving under influence of alcohol or drugs</li>
        <li>Using mobile phone while driving (hands-free permitted)</li>
        <li>Allowing unauthorized persons to drive</li>
        <li>Subletting or re-renting the vehicle</li>
        <li>Transporting more passengers than seatbelts available</li>
        <li>Smoking in the vehicle</li>
        </ul>
        <h3 style='color:#1e88e5;'>8. BREAKDOWN AND ACCIDENTS</h3>
        <b>8.1 Emergency Procedures</b>
        <ol>
        <li>Ensure safety of all passengers</li>
        <li>Contact emergency services if needed</li>
        <li>Contact Royal Car Rentals immediately</li>
        <li>Do not admit fault or liability</li>
        <li>Obtain police report if required</li>
        </ol>
        <b>8.2 Roadside Assistance</b>
        <p>24/7 roadside assistance included for mechanical breakdowns, flat tires, and lockouts within service area.</p>
        <h3 style='color:#1e88e5;'>9. DATA PROTECTION AND PRIVACY</h3>
        <b>9.1 Information Collection</b>
        <p>We collect personal information necessary for rental service including driving record verification and credit checks.</p>
        <b>9.2 Data Usage</b>
        <ul>
        <li>Processing rentals</li>
        <li>Fraud prevention</li>
        <li>Marketing communications (with consent)</li>
        <li>Legal compliance</li>
        </ul>
        <h3 style='color:#1e88e5;'>10. DISPUTE RESOLUTION</h3>
        <b>10.1 Governing Law</b>
        <p>These terms are governed by the laws of the jurisdiction where rental occurs.</p>
        <b>10.2 Dispute Process</b>
        <p>Disputes should first be addressed through our customer service. Unresolved disputes may be subject to binding arbitration.</p>
        <h3 style='color:#1e88e5;'>11. MODIFICATIONS AND UPDATES</h3>
        <p>Royal Car Rentals reserves the right to modify these terms at any time. Updated terms will be posted on our website and apply to future rentals.</p>
        <h3 style='color:#1e88e5;'>12. CONTACT INFORMATION</h3>
        <p>
        For questions about these terms:<br>
        <b>Phone:</b> 1-800-ROYAL-CAR<br>
        <b>Email:</b> support@royalcarrentals.com<br>
        <b>Website:</b> www.royalcarrentals.com
        </p>
        <p style='font-size:16px; color:#1e88e5;'><b>By renting from Royal Car Rentals, you acknowledge that you have read, understood, and agree to be bound by these Terms and Conditions.</b></p>
        </div>
    )";

    QLabel* termsLabel = new QLabel(termsText);
    termsLabel->setStyleSheet(
        "background: rgba(30,136,229,0.10);"
        "border-radius: 18px;"
        "padding: 30px;"
        "margin-top: 16px;"
        "color: white;"
        "font-size: 15px;"
        );
    termsLabel->setWordWrap(true);
    termsLabel->setAlignment(Qt::AlignLeft);
    termsLabel->setTextFormat(Qt::RichText);

    layout->addWidget(title);
    layout->addWidget(line);
    layout->addWidget(termsLabel);

    content->setStyleSheet("background: transparent;");
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; background-color: #0D0D0D; }"
        "QWidget { background: transparent; }"
        );

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(scrollArea);

    page->setStyleSheet("background: #0D0D0D;");
    return page;
}

QWidget* CustomerDashboard::createSettingsPage()
{
    QWidget* page = new QWidget;
    page->setStyleSheet("background-color: #121a27; color: white; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;");

    QVBoxLayout* mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(30);

    // Title
    QLabel* title = new QLabel("⚙ Account Settings");
    title->setStyleSheet("font-size: 36px; font-weight: 900; color: #1e88e5;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Horizontal line
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(3);
    line->setStyleSheet("background-color: #1e88e5; border:none;");
    mainLayout->addWidget(line);

    // Content layout (two columns)
    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setSpacing(60);

    // Left column: Profile Picture + Actions
    QWidget* leftCol = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout(leftCol);
    leftLayout->setSpacing(40);

    // Avatar and upload
    QLabel* avatar = new QLabel;
    avatar->setFixedSize(120, 120);
    avatar->setStyleSheet("border-radius: 60px; border: 4px solid #1e88e5; background: #cfd8dc;");
    QPixmap avatarPixmap(":/icons/user.png"); // Default avatar
    avatar->setPixmap(avatarPixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    avatar->setAlignment(Qt::AlignCenter);

    QPushButton* uploadBtn = new QPushButton("Change Photo");
    uploadBtn->setCursor(Qt::PointingHandCursor);
    uploadBtn->setFixedSize(160, 42);
    uploadBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #1e88e5;
            color: white;
            font-weight: 700;
            font-size: 15px;
            border-radius: 10px;
            padding: 10px 0;
        }
        QPushButton:hover {
            background-color: #1565c0;
        }
    )");

    connect(uploadBtn, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(page, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg)");
        if (!fileName.isEmpty()) {
            QPixmap newPic(fileName);
            if (!newPic.isNull()) {
                avatar->setPixmap(newPic.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                // TODO: Save avatar persistently
            }
        }
    });

    leftLayout->addWidget(avatar, 0, Qt::AlignHCenter);
    leftLayout->addWidget(uploadBtn, 0, Qt::AlignHCenter);

    // Manage Credit Cards Button
    QPushButton* manageCardsBtn = new QPushButton("Manage Credit Cards");
    manageCardsBtn->setCursor(Qt::PointingHandCursor);
    //manageCardsBtn->setFixedHeight(44);
    manageCardsBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #1e88e5;
            color: white;
            font-weight: 400;
            font-size: 12px;
            border-radius: 24px;
            padding: 10px 24px;
            margin-top: 20px;
            width: 100%;
        }
        QPushButton:hover {
            background-color: #1565c0;
        }
    )");
    leftLayout->addWidget(manageCardsBtn);

    // Delete Account Button
    QPushButton* deleteBtn = new QPushButton("Delete My Account");
    deleteBtn->setCursor(Qt::PointingHandCursor);
    //deleteBtn->setFixedHeight(44);
    deleteBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #f44336;
            color: white;
            font-weight: 400;
            font-size: 12px;
            border-radius: 24px;
            padding: 10px 24px;
            margin-top: 20px;
            width: 100%;
        }
        QPushButton:hover {
            background-color: #d32f2f;
        }
    )");
    leftLayout->addWidget(deleteBtn);

    contentLayout->addWidget(leftCol, 0);

    // Right column: Form fields with QFormLayout for alignment
    QWidget* rightCol = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightCol);
    rightLayout->setSpacing(30);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    formLayout->setHorizontalSpacing(20);
    formLayout->setVerticalSpacing(18);

    // Full Name
    QLineEdit* nameEdit = new QLineEdit;
    nameEdit->setText(currentCustomer->getName());
    nameEdit->setStyleSheet("background-color: #232e3b; color: white; border-radius: 8px; padding: 12px; font-size: 16px;");
    formLayout->addRow("Full Name:", nameEdit);

    // Email
    QLineEdit* emailEdit = new QLineEdit;
    emailEdit->setText(currentCustomer->getEmail());
    emailEdit->setStyleSheet("background-color: #232e3b; color: white; border-radius: 8px; padding: 12px; font-size: 16px;");
    emailEdit->setReadOnly(true); // Email usually not editable
    formLayout->addRow("Email Address:", emailEdit);

    // Password
    QLineEdit* passEdit = new QLineEdit;
    passEdit->setText(currentCustomer->getPassword());
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setStyleSheet("background-color: #232e3b; color: white; border-radius: 8px; padding: 12px; font-size: 16px;");
    formLayout->addRow("Password:", passEdit);

    // Contact Number
    QLineEdit* contactEdit = new QLineEdit;
    contactEdit->setText(currentCustomer->getContact());
    contactEdit->setStyleSheet("background-color: #232e3b; color: white; border-radius: 8px; padding: 12px; font-size: 16px;");
    formLayout->addRow("Contact Number:", contactEdit);

    // Billing Address
    QLineEdit* billingEdit = new QLineEdit;
    billingEdit->setText(currentCustomer->getBillingAddress());
    billingEdit->setStyleSheet("background-color: #232e3b; color: white; border-radius: 8px; padding: 12px; font-size: 16px;");
    formLayout->addRow("Billing Address:", billingEdit);

    rightLayout->addLayout(formLayout);
    rightLayout->addStretch();

    // Action Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(25);

    QPushButton* saveBtn = new QPushButton("Save Changes");
    saveBtn->setStyleSheet("background-color: #43a047; color: white; font-weight: 700; font-size: 16px; border-radius: 12px; padding: 10px 28px;");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    cancelBtn->setStyleSheet("background-color: #e53935; color: white; font-weight: 700; font-size: 16px; border-radius: 12px; padding: 10px 28px;");
    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet("background-color: #1e88e5; color: white; font-weight: 700; font-size: 16px; border-radius: 12px; padding: 10px 28px;");

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(logoutBtn);

    rightLayout->addLayout(buttonLayout);

    contentLayout->addWidget(rightCol, 1);

    mainLayout->addLayout(contentLayout);

    // --- Signals (Preserve your logic) ---
    connect(saveBtn, &QPushButton::clicked, [=]() {
        if (nameEdit->text().isEmpty() || contactEdit->text().isEmpty()) {
            QMessageBox::warning(page, "Error", "Please fill in all required fields.");
            return;
        }
        DatabaseManager::updateCustomer(
            currentCustomer->getId(),
            nameEdit->text(),
            passEdit->text(),
            contactEdit->text()
            );
        currentCustomer->setName(nameEdit->text());
        currentCustomer->setPassword(passEdit->text());
        currentCustomer->setContact(contactEdit->text());
        currentCustomer->setBillingAddress(billingEdit->text());
        QMessageBox::information(page, "Success", "Profile updated successfully!");
    });

    connect(cancelBtn, &QPushButton::clicked, [=]() {
        nameEdit->setText(currentCustomer->getName());
        passEdit->setText(currentCustomer->getPassword());
        contactEdit->setText(currentCustomer->getContact());
        billingEdit->setText(currentCustomer->getBillingAddress());
    });

    connect(logoutBtn, &QPushButton::clicked, this, &CustomerDashboard::logout);

    return page;
}


void CustomerDashboard::loadCarsFromDatabase()
{
    // This would typically load from your database
    // For now, I'll create sample data that matches the design
    allCars = {
        {1, "Porsche 718 Cayman S", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard1.jpg", 2, "Manual", 400, 4.5, true},
        {2, "Mini Cooper 5-DOOR", "Hatchback", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard2.jpg", 4, "Automatic", 364, 4.2, true},
        {3, "Toyota GR Supra", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard3.jpg", 2, "Manual", 360, 4.7, true},
        {4, "Porsche 911 Turbo", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard8.jpg", 2, "Manual", 468, 4.8, true},
        {5, "Porsche Taycan 4S", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard9.jpg", 2, "Automatic", 424, 4.6, true},
        {6, "Mini Cooper Works", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard10.jpg", 4, "Automatic", 360, 4.3, true},
        {7, "Mercedes Amg", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard4.jpg", 2, "Manual", 468, 4.8, true},
        {8, "Mercedes G-Wagon", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard5.jpg", 2, "Automatic", 424, 4.6, true},
        {9, "BMW M8", "Coupe", "C:/Users/AL RASHIDS/Documents/RoyalCarRental/assets/carcard6.jpg", 4, "Automatic", 360, 4.3, true}
    };


    filteredCars = allCars;
}

void CustomerDashboard::displayCars(const QList<CarData>& cars)
{
    // Clear existing layout
    if (carContainer->layout()) {
        QLayoutItem* item;
        while ((item = carContainer->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete carContainer->layout();
    }

    QGridLayout* carLayout = new QGridLayout(carContainer);
    carLayout->setSpacing(15);

    for (int i = 0; i < cars.size(); ++i) {
        QWidget* carCard = createCarCard(cars[i]);
        carLayout->addWidget(carCard, i / 3, i % 3);
    }

    carContainer->setLayout(carLayout);
}

void CustomerDashboard::applyFilters()
{
    // Apply filtering logic based on selected filters
    // This is a simplified version - you would implement full filtering
    displayCars(allCars);
}

void CustomerDashboard::showBookingPage() { contentStack->setCurrentIndex(1); }
void CustomerDashboard::showAboutPage() { contentStack->setCurrentIndex(2); }
void CustomerDashboard::showTermsPage() { contentStack->setCurrentIndex(3); }
void CustomerDashboard::showSettingsPage() { contentStack->setCurrentIndex(4); }
void CustomerDashboard::searchCars() { applyFilters(); }
void CustomerDashboard::filterCars() { applyFilters(); }

void CustomerDashboard::handleRentClicked(const CarData& car)
{
    // For now, just show the rent dialog
    showRentDialog(car);
}

void CustomerDashboard::logout()
{
    if (loginRef) {
        this->close();
        loginRef->show();
    }
}
