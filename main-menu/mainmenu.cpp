#include "mainmenu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setStyleSheet("background-color: black; color: white;");
}

void MainMenu::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title
    QLabel *title = new QLabel("ROGUEX86", this);
    title->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QFont titleFont = title->font();
    titleFont.setPointSize(48);
    title->setFont(titleFont);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *btnOptions = createMenuButton("Options");
    QPushButton *btnNewGame = createMenuButton("New Game");
    QPushButton *btnExit = createMenuButton("Exit");

    buttonLayout->addWidget(btnOptions);
    buttonLayout->addWidget(btnNewGame);
    buttonLayout->addWidget(btnExit);

    mainLayout->addWidget(title);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(btnOptions, &QPushButton::clicked, this, &MainMenu::showOptions);
    connect(btnExit, &QPushButton::clicked, qApp, &QApplication::quit);
}

QPushButton *MainMenu::createMenuButton(const QString &text)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setFixedSize(150, 50);
    btn->setStyleSheet("QPushButton { background-color: #555; color: white; }");
    return btn;
}
