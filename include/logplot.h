#ifndef NETWORKPLOT_H
#define NETWORKPLOT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QTimer>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LogPlot;
}
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

    QVector<QString> stdVectorString2qVectorString(const std::vector<std::string> &stdVec)
    {
        QVector<QString> qVec;

        for (const std::string &value : stdVec)
        {
            qVec.push_back(QString::fromStdString(value));
        }

        return qVec;
    }

    double convertInBoundary(const std::string &strNumber)
    {
        double number;
        try {
            number = std::stod(strNumber); // 尝试将字符串转换为 double
        }
        catch (std::invalid_argument &) {
            return 0.0; // 或者返回其他默认值
        }
        catch (std::out_of_range &)
        {
            if (strNumber[0] == '-') {
                return -std::numeric_limits<double>::max();
            }
            else {
                return std::numeric_limits<double>::max();
            }
        }

        if (number > std::numeric_limits<double>::max()) {
            return std::numeric_limits<double>::max();
        }
        else if (number < -std::numeric_limits<double>::max()) {
            return -std::numeric_limits<double>::max();
        }
        else {
            return number;
        }
    }

    Ui::LogPlot *ui;
    QString folderPath;
    int normal_graph_num;
    QVector<QColor> color_list;
    QVector<QString> label_list;

    bool data_ready{false}; // 等待数据读取完毕
    bool graph_changed{false};
    QVector<bool> is_plot;
    QVector<QVector<double>> all_data;
    QVector<double> time_x;
    QVector<QCheckBox *> checkboxes;
};
#endif // NETWORKPLOT_H
