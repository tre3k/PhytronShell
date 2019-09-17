#include <QApplication>

#include "maindialog.h"

int main(int argc,char **argv){
    QApplication a(argc,argv);

    auto main_dialog = new MainDialog();
    main_dialog->show();


    return a.exec();
}
