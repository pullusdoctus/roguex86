#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class MainMenu;
class OptionsMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    MainMenu *mainMenu;
    OptionsMenu *optionsMenu;
};

#endif // MAINWINDOW_H
