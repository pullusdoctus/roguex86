#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QWidget>

class QPushButton;

class OptionsMenu : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsMenu(QWidget *parent = nullptr);

signals:
    void backToMain();

private:
    void setupUI();
    QPushButton *createMenuButton(const QString &text);
};

#endif // OPTIONSMENU_H
