#include <QApplication>
#include "demo.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Demo demo;
    demo.show();

    return app.exec();
}
