#include "mainwindow.h"
#include "mainmenu.h"
#include "optionsmenu.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("roguex86");
    setFixedSize(1280, 720); // 16:9 resolution

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    mainMenu = new MainMenu(this);
    optionsMenu = new OptionsMenu(this);

    stackedWidget->addWidget(mainMenu);
    stackedWidget->addWidget(optionsMenu);

    // Connect signals
    connect(mainMenu, &MainMenu::showOptions, [this]() {
        stackedWidget->setCurrentWidget(optionsMenu);
    });
    connect(optionsMenu, &OptionsMenu::backToMain, [this]() {
        stackedWidget->setCurrentWidget(mainMenu);
    });
}
