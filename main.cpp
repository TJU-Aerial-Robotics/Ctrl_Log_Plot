#include "logplot.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogPlot plt;
    plt.show();
    return a.exec();
}
