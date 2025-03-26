#include "demo.h"
#include "./ui_demo.h"

Demo::Demo(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void Demo::setupUI() {
    // Create widgets
    QLabel *titleLabel = new QLabel("Qt Widgets Demo", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    QPushButton *messageButton = new QPushButton("Show Message", this);
    QPushButton *closeButton = new QPushButton("Exit", this);
    QLineEdit *inputField = new QLineEdit(this);
    QCheckBox *checkBox = new QCheckBox("Enable Feature", this);
    QRadioButton *radio1 = new QRadioButton("Option 1", this);
    QRadioButton *radio2 = new QRadioButton("Option 2", this);
    QComboBox *comboBox = new QComboBox(this);
    QProgressBar *progressBar = new QProgressBar(this);
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    statusLabel = new QLabel("Status: Ready", this);

    // Set up combo box
    comboBox->addItems({"Easy", "Medium", "Hard"});

    // Set up progress bar and slider
    progressBar->setRange(0, 100);
    slider->setRange(0, 100);

    // Create layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *radioLayout = new QHBoxLayout();

    // Add widgets to layouts
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(inputField);
    mainLayout->addWidget(checkBox);

    radioLayout->addWidget(radio1);
    radioLayout->addWidget(radio2);
    mainLayout->addLayout(radioLayout);

    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(slider);
    mainLayout->addWidget(statusLabel);

    buttonLayout->addWidget(messageButton);
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    // Store widgets we need to access later
    this->inputField = inputField;
    this->progressBar = progressBar;
    this->slider = slider;
    this->checkBox = checkBox;
    this->messageButton = messageButton;
    this->closeButton = closeButton;

    // Style the window
    setWindowTitle("Qt Demo");
    setFixedSize(400, 400);
}

void Demo::setupConnections() {
    // Connect buttons
    connect(messageButton, &QPushButton::clicked,
            this, &Demo::showMessage);
    connect(closeButton, &QPushButton::clicked,
            qApp, &QApplication::quit);

    // Connect input field
    connect(inputField, &QLineEdit::textChanged, [this](const QString &text) {
        statusLabel->setText("You typed: " + text);
    });

    // Connect checkbox
    connect(checkBox, &QCheckBox::stateChanged, [this](int state) {
        inputField->setEnabled(state == Qt::Checked);
    });

    // Connect slider to progress bar
    connect(slider, &QSlider::valueChanged,
            progressBar, &QProgressBar::setValue);
}

void Demo::showMessage() {
    QMessageBox::information(this, "Hello!",
                             "This is a Qt message box!\n"
                             "Current input: " + inputField->text());
}
