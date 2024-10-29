/*
 * 文件名: logplot.cpp
 * 作者: lu junjie
 * 创建日期: 2024年3月6日
 * 描述: plot窗口左键框选缩放、右键平移，plot相关问题, 记住打开的文件夹，窗口缩放跟随问题，（立文）根据日志更新label
 */

#include "logplot.h"
#include "ui_logplot.h"

LogPlot::LogPlot(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LogPlot)
{
    ui->setupUi(this);
    // 构建所有label------------------
    color_list = {QColor("#D2042D"), QColor("#00008B"), QColor("#4666FF"),
                  QColor("#FFA500"), QColor("#FF00FF"), QColor("#006DB0"),
                  QColor("#00FFFF"), QColor("#40B5AD"), QColor("#FF7A00"), QColor("#71797E"),
                  QColor("#93C572"), QColor("#009E60"), QColor("#C2B280"),
                  QColor("#DAA520"), QColor("#9F2B68"), QColor("#DA70D6"),
                  QColor("#800080"), QColor("#301934"), QColor("#E0B0FF"), QColor("#669900"),
                  QColor("#9933CC"), QColor("#CCCC00"), QColor("#996699"),
                  QColor("#009B7D"), QColor("#FF4F00"), QColor("#319177"),
                  QColor("#5A4FCF"), QColor("#1164B4"), QColor("#FCF55F"), QColor("#9966FF")};

    checkboxes = {ui->est_px, ui->est_py, ui->est_pz,
                  ui->est_vx, ui->est_vy, ui->est_vz,
                  ui->est_ax, ui->est_ay, ui->est_az, ui->est_yaw,
                  ui->des_px, ui->des_py, ui->des_pz,
                  ui->des_vx, ui->des_vy, ui->des_vz,
                  ui->des_ax, ui->des_ay, ui->des_az, ui->des_yaw,
                  ui->err_px, ui->err_py, ui->err_pz,
                  ui->err_vx, ui->err_vy, ui->err_vz,
                  ui->err_ax, ui->err_ay, ui->err_az, ui->err_yaw};

    for (unsigned i = 0; i < checkboxes.size(); i++)
        is_plot.push_back(false);
    // ---------------------------------

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Value");
    ui->customPlot->xAxis->setRange(0, 100);
    ui->customPlot->yAxis->setRange(0, 10);

    // 设置模式，放大还是平移
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
    ui->customPlot->selectionRect()->setPen(QPen(Qt::black, 1, Qt::DashLine)); // 虚线
    ui->customPlot->selectionRect()->setBrush(QBrush(QColor(0, 0, 100, 50)));  // 半透明浅蓝

    folderPath = QDir::homePath();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LogPlot::plot_timer_callback);
    timer->start(200); // 每0.2秒触发一次timeout信号
}

LogPlot::~LogPlot()
{
    delete ui;
}

void LogPlot::on_openButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), folderPath);
    if (!filePath.isEmpty())
    {
        data_ready = false;
        folderPath = filePath.section('/', 0, -2);
        ui->fileName->setText(filePath);

        if (!process_data(filePath))
        {
            ui->fileName->setText("Wrong File!");
            return;
        }

        data_ready = true;
        graph_changed = true;
    }
}

/**
 * @description: 定时器，一旦读取完成且对话框有变化，就刷新一下画图，不进行缩放
 */
void LogPlot::plot_timer_callback()
{
    if (!data_ready)
        return;

    for (unsigned i = 0; i < checkboxes.size(); i++)
    {
        if (checkboxes[i]->isChecked())
        {
            if (is_plot[i] == false)
                graph_changed = true;
            is_plot[i] = true;
        }
        else
        {
            if (is_plot[i] == true)
                graph_changed = true;
            is_plot[i] = false;
        }
    }

    if (!graph_changed)
        return;

    int graph_id = 0;
    ui->customPlot->clearGraphs();
    for (unsigned i = 0; i < checkboxes.size(); i++)
    {
        if (is_plot[i])
        {
            ui->customPlot->addGraph();
            ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssNone);
            ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::lsLine);
            QPen pen;
            pen.setWidth(3);             // 设置线宽
            pen.setColor(color_list[i]); // 设置线条色
            ui->customPlot->graph(graph_id)->setPen(pen);
            ui->customPlot->graph(graph_id)->setName(label_list[i]); // 设置图例
            ui->customPlot->graph(graph_id)->setData(time_x, all_data[i]);
            graph_id++;
        }
    }

    ui->customPlot->replot();
    ui->customPlot->update();
    graph_changed = false;
}

/**
 * @description: 重新画图并缩放合适
 */
void LogPlot::on_plotButton_clicked()
{
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    ui->customPlot->update();
}

/**
 * @description: 清除所有图表，将对钩设成空
 */
void LogPlot::on_clearButton_clicked()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
    ui->customPlot->update();
    for (size_t i = 0; i < checkboxes.size(); i++)
    {
        checkboxes[i]->setChecked(false);
    }
    graph_changed = false;
    for (size_t i = 0; i < checkboxes.size(); i++)
    {
        is_plot[i] = false;
    }
}

// 小心段错误
bool LogPlot::process_data(QString file_path_)
{
    try
    {
        std::string file_path = file_path_.toStdString();
        std::vector<std::vector<std::string>> temp;
        std::ifstream file(file_path);
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string value;
            while (std::getline(ss, value, ','))
            {
                row.push_back(value);
            }
            temp.push_back(row);
        }
        file.close();

        // 小心内存溢出
        all_data.clear();
        time_x.clear();
        all_data.resize(checkboxes.size());
        std::vector<std::string> label = temp[0];
        QVector<QString> label_list_temp;
        QVector<QString> qlabel = stdVectorString2qVectorString(label);
        for (size_t i = 1; i < checkboxes.size() + 1; i++)
        {
            if (i < qlabel.size())
            {
                checkboxes[i - 1]->setCheckable(true);
                checkboxes[i - 1]->setText(qlabel[i]);
                label_list_temp.push_back(qlabel[i]);
            }
            else
            {
                checkboxes[i - 1]->setCheckable(false);
                checkboxes[i - 1]->setText("unused");
            }
        }
        QVector<QVector<double>> real_data;
        ;
        for (unsigned j = 1; j < temp[0].size(); j++)
        {
            QVector<double> column;
            for (unsigned i = 1; i < temp.size(); i++)
            {
                column.push_back(convertInBoundary(temp[i][j]));
            }
            real_data.push_back(column);
        }
        for (unsigned i = 1; i < temp.size(); i++)
        {
            double time_i = (std::stod(temp[i][0]) - std::stod(temp[1][0])) / 1000000000;
            time_x.push_back(time_i);
        }

        all_data = real_data;
        label_list = label_list_temp;

        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}