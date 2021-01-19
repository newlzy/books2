#include <QApplication>
#include <QtWidgets>
#include "bookwindow.h"

int main(int argc ,char *argv[]){

    //初始化qrc资源。通常，当资源作为应用程序的一部分构建时，资源在启动时自动加载。
    Q_INIT_RESOURCE(bookss);
    QApplication app(argc,argv);
    BookWindow win;
    win.show();

    return app.exec();

}

