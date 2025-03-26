#ifndef DEMO_H
#define DEMO_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Demo;
}
QT_END_NAMESPACE

class Demo : public QMainWindow
{
    Q_OBJECT

public:
    Demo(QWidget *parent = nullptr);
    ~Demo();

private:
    Ui::Demo *ui;
};
#endif // DEMO_H
