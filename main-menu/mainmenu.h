#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

class QPushButton;

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);

signals:
    void showOptions();
    void exitGame();

private:
    void setupUI();
    QPushButton *createMenuButton(const QString &text);
};

#endif // MAINMENU_H
