#ifndef DEMO_H
#define DEMO_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QProgressBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

class Demo : public QWidget {
    Q_OBJECT

public:
    explicit Demo(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();

private slots:
    void showMessage();

private:
    QLineEdit *inputField;
    QProgressBar *progressBar;
    QSlider *slider;
    QCheckBox *checkBox;
    QLabel *statusLabel;
    QPushButton *messageButton;
    QPushButton *closeButton;
};

//#include "demo.moc"
#endif
