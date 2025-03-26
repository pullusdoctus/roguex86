#include "optionsmenu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

OptionsMenu::OptionsMenu(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setStyleSheet("background-color: black; color: white;");
}

void OptionsMenu::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title
    QLabel *title = new QLabel("OPTIONS", this);
    title->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QFont titleFont = title->font();
    titleFont.setPointSize(48);
    title->setFont(titleFont);

    // Options buttons
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    QPushButton *btnVolume = createMenuButton("Volume");
    QPushButton *btnGraphics = createMenuButton("Graphics");
    QPushButton *btnControls = createMenuButton("Controls");

    optionsLayout->addWidget(btnVolume);
    optionsLayout->addWidget(btnGraphics);
    optionsLayout->addWidget(btnControls);

    // Back button
    QPushButton *btnBack = createMenuButton("Back");
    btnBack->setFixedSize(100, 40);

    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(btnBack);
    backLayout->addStretch();

    mainLayout->addWidget(title);
    mainLayout->addLayout(optionsLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(backLayout);

    connect(btnBack, &QPushButton::clicked, this, &OptionsMenu::backToMain);
}

QPushButton *OptionsMenu::createMenuButton(const QString &text)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setFixedSize(150, 50);
    btn->setStyleSheet("QPushButton { background-color: #555; color: white; }");
    return btn;
}
