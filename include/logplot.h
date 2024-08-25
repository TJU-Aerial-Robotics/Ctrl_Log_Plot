#ifndef NETWORKPLOT_H
#define NETWORKPLOT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "pid_analyzer_interface.h"
#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QFileDialog>
#include <QTimer>
#include "qcustomplot.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LogPlot; }
QT_END_NAMESPACE

class LogPlot : public QMainWindow
{
    Q_OBJECT

public:
    LogPlot(QWidget *parent = nullptr);
    ~LogPlot();

private slots:
    void on_plotButton_clicked();

    void on_clearButton_clicked();

    void on_openButton_clicked();

private:
    void plot_timer_callback();

    bool process_data(QString path);

    Ui::LogPlot *ui;
    QString folderPath;
    int graph_num;
    QVector<QColor> color_list;
    QVector<QString> label_list;

    bool data_ready{false}; // 等待数据读取完毕
    bool graph_changed{false};
    QVector<bool> is_plot;
    QVector<QVector<double>> all_data;
    QVector<double> time_x;
    QVector<QCheckBox*> checkboxes;
};
#endif // NETWORKPLOT_H
