#include <QApplication>
#include <QDir>
#include "LoginWindow.h"
#include "DatabaseManager.h"
#include "RentalSystem.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Optional: Ensure working directory is app dir
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // Step 1: Connect to DB
    if (!DatabaseManager::connect()) {
        qFatal("❌ Failed to connect to SQLite database.");
        return -1;
    }

    DatabaseManager::printAllUsers();

    // Step 2: Create tables if missing
    DatabaseManager::runSeedIfNeeded();

    // Step 3: Ensure fallback admin exists
    DatabaseManager::ensureFallbackAdmin();

    // Step 4: Preload any memory-cached data
    RentalSystem::getInstance();

    // Step 5: Launch login
    LoginWindow login;
    login.setWindowTitle("Royal Car Rentals - Login");
    login.resize(800, 600);
    login.show();

    return app.exec();
}
