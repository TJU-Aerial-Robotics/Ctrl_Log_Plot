/*
 * 文件名: logplot.cpp
 * 作者: lu junjie
 * 创建日期: 2024年3月6日
 * 描述: plot窗口左键框选缩放、右键平移，plot相关问题, 记住打开的文件夹，窗口缩放跟随问题

 * 修改日期: 2024年8月24日
 * 描述: 添加单位阶跃响应曲线 和 误差曲线， 修改颜色
 */

#include "logplot.h"
#include "ui_logplot.h"

LogPlot::LogPlot(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LogPlot)
{
    ui->setupUi(this);
    // 构建所有label------------------
    color_list = {QColor("#D2042D"), QColor("#00008B"), QColor("#4666FF"),
                  QColor("#FFA500"), QColor("#FF00FF"), QColor("#006DB0"),
                  QColor("#00FFFF"), QColor("#40B5AD"), QColor("#FF7A00"), QColor("#71797E"),

                  QColor("#93C572"), QColor("#009E60"), QColor("#C2B280"),
                  QColor("#00FFFF"), QColor("#9F2B68"), QColor("#DA70D6"),
                  QColor("#800080"), QColor("#301934"), QColor("#E0B0FF"),

                  QColor("#71797E"), QColor("#D2042D"), QColor("#00008B"), 
                  QColor("#009B7D"), QColor("#FF4F00"), QColor("#319177"),
                  QColor("#5A4FCF"), QColor("#1164B4"), QColor("#FCF55F"),

                  QColor("#93C572"), QColor("#009E60"), QColor("#C2B280"),
                  QColor("#DAA520"), QColor("#9F2B68"), QColor("#DA70D6"),
                  QColor("#800080"), QColor("#301934"), QColor("#E0B0FF")};
    label_list = {"est_p(x)", "est_p(y)", "est_p(z)",
                  "est_v(x)", "est_v(y)", "est_v(z)",
                  "est_a(x)", "est_a(y)", "est_a(z)", "est_yaw",
                  "des_p(x)", "des_p(y)", "des_p(z)",
                  "des_v(x)", "des_v(y)", "des_v(z)",
                  "des_a(x)", "des_a(y)", "des_a(z)",
                  "err_p(x)", "err_p(y)", "err_p(z)",
                  "err_v(x)", "err_v(y)", "err_v(z)",
                  "err_a(x)", "err_a(y)", "err_a(z)",
                  "response_p(x)", "response_p(y)", "response_p(z)",
                  "response_v(x)", "response_v(y)", "response_v(z)",};
    checkboxes = {ui->est_px, ui->est_py, ui->est_pz,
                  ui->est_vx, ui->est_vy, ui->est_vz,
                  ui->est_ax, ui->est_ay, ui->est_az, ui->est_yaw,
                  ui->des_px, ui->des_py, ui->des_pz,
                  ui->des_vx, ui->des_vy, ui->des_vz,
                  ui->des_ax, ui->des_ay, ui->des_az,
                  ui->err_px, ui->err_py, ui->err_pz,
                  ui->err_vx, ui->err_vy, ui->err_vz,
                  ui->err_ax, ui->err_ay, ui->err_az,
                  ui->ana_px, ui->ana_py, ui->ana_pz,
                  ui->ana_vx, ui->ana_vy, ui->ana_vz};
    
    normal_graph_num = 28;  // p v a yaw
    response_graph_num = 6; // response p v
    for (size_t i = 0; i < normal_graph_num + response_graph_num; i++)
        is_plot.push_back(false);
    // ---------------------------------

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Value");
    ui->customPlot->xAxis->setRange(0,100);
    ui->customPlot->yAxis->setRange(0,10);

    // 设置模式，放大还是平移
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
    ui->customPlot->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//虚线
    ui->customPlot->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));//半透明浅蓝    

    folderPath = QDir::homePath();

    QTimer* timer = new QTimer(this);
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
void LogPlot::plot_timer_callback(){
    if(!data_ready)
        return;

    for (size_t i = 0; i < checkboxes.size(); i++)
    {
        if (checkboxes[i]->isChecked()){
            if (is_plot[i] == false)
                graph_changed = true;
            is_plot[i] = true;
        }
        else{
            if (is_plot[i] == true)
                graph_changed = true;
            is_plot[i] = false;
        }
    }
    
    if(!graph_changed)
        return;

    int graph_id = 0;
    ui->customPlot->clearGraphs();
    for (size_t i = 0; i < normal_graph_num; i++)
    {
        if (is_plot[i])
        {
            ui->customPlot->addGraph();
            ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssNone);
            ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::lsLine);
            QPen pen;
            pen.setWidth(3);//设置线宽
            pen.setColor(color_list[i]);//设置线条色
            ui->customPlot->graph(graph_id)->setPen(pen);
            ui->customPlot->graph(graph_id)->setName(label_list[i]);      // 设置图例
            ui->customPlot->graph(graph_id)->setData(time_x, all_data[i]);
            graph_id ++;
        }
    }
    bool response_plot = false;
    for (size_t i = normal_graph_num; i < normal_graph_num + response_graph_num; i++)
    {
        if (is_plot[i])
        {
            ui->customPlot->addGraph();
            ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssNone);
            ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::lsLine);
            QPen pen;
            pen.setWidth(3);
            pen.setColor(color_list[i]);
            ui->customPlot->graph(graph_id)->setPen(pen);
            ui->customPlot->graph(graph_id)->setName(label_list[i]);
            ui->customPlot->graph(graph_id)->setData(response_time, all_data[i]);
            response_plot = true;
            graph_id ++;
        }
    }
    if (response_plot)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssNone);
        ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::lsLine);
        QPen pen;
        pen.setWidth(3);
        pen.setColor(color_list[0]);
        ui->customPlot->graph(graph_id)->setPen(pen);
        ui->customPlot->graph(graph_id)->setName("step ref");
        ui->customPlot->graph(graph_id)->setData(response_time, response_ref);
        graph_id ++;
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
    for (size_t i = 0; i < normal_graph_num; i++){
        is_plot[i] = false;
    }
}

// 小心段错误
bool LogPlot::process_data(QString file_path_) {
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
        all_data.resize(normal_graph_num);
        std::vector<std::string> label = temp[0];
        QVector<double> est_px, est_py, est_pz, est_vx, est_vy, est_vz, est_ax, est_ay, est_az, est_yaw;
        QVector<double> des_px, des_py, des_pz, des_vx, des_vy, des_vz, des_ax, des_ay, des_az;
        QVector<double> err_px, err_py, err_pz, err_vx, err_vy, err_vz, err_ax, err_ay, err_az;
        QVector<double> response_px, response_py, response_pz, response_vx, response_vy, response_vz;
        for (int i = 1; i < temp.size(); i++)
        {   
            if (temp[i].size()<23)
                return false;
            
            est_px.push_back(std::stod(temp[i][1]));
            est_py.push_back(std::stod(temp[i][2]));
            est_pz.push_back(std::stod(temp[i][3]));
            est_vx.push_back(std::stod(temp[i][4]));
            est_vy.push_back(std::stod(temp[i][5]));
            est_vz.push_back(std::stod(temp[i][6]));
            est_ax.push_back(std::stod(temp[i][7]));
            est_ay.push_back(std::stod(temp[i][8]));
            est_az.push_back(std::stod(temp[i][9]));
            est_yaw.push_back(std::stod(temp[i][10]));

            des_px.push_back(std::stod(temp[i][14]));
            des_py.push_back(std::stod(temp[i][15]));
            des_pz.push_back(std::stod(temp[i][16]));
            des_vx.push_back(std::stod(temp[i][17]));
            des_vy.push_back(std::stod(temp[i][18]));
            des_vz.push_back(std::stod(temp[i][19]));
            des_ax.push_back(std::stod(temp[i][20]));
            des_ay.push_back(std::stod(temp[i][21]));
            des_az.push_back(std::stod(temp[i][22]));
            
            err_px.push_back(des_px.back() - est_px.back());
            err_py.push_back(des_py.back() - est_py.back());
            err_pz.push_back(des_pz.back() - est_pz.back());
            err_vx.push_back(des_vx.back() - est_vx.back());
            err_vy.push_back(des_vy.back() - est_vy.back());
            err_vz.push_back(des_vz.back() - est_vz.back());
            err_ax.push_back(des_ax.back() - est_ax.back());
            err_ay.push_back(des_ay.back() - est_ay.back());
            err_az.push_back(des_az.back() - est_az.back());

            double time_i = (std::stod(temp[i][0]) - std::stod(temp[1][0])) / 1000000000;
            time_x.push_back(time_i);
        }
        {
            std::vector<double> time_temp, response_time_temp, response_px_temp, response_py_temp, response_pz_temp, response_vx_temp, response_vy_temp, response_vz_temp;
            std::vector<double> est_px_temp, est_py_temp, est_pz_temp, est_vx_temp, est_vy_temp, est_vz_temp;
            std::vector<double> des_px_temp, des_py_temp, des_pz_temp, des_vx_temp, des_vy_temp, des_vz_temp;
            time_temp = time_x.toStdVector();
            est_px_temp = est_px.toStdVector();
            des_px_temp = des_px.toStdVector();
            est_py_temp = est_py.toStdVector();
            des_py_temp = des_py.toStdVector();
            est_pz_temp = est_pz.toStdVector();
            des_pz_temp = des_pz.toStdVector();
            est_vx_temp = est_vx.toStdVector();
            des_vx_temp = des_vx.toStdVector();
            est_vy_temp = est_vy.toStdVector();
            des_vy_temp = des_vy.toStdVector();
            est_vz_temp = est_vz.toStdVector();
            des_vz_temp = des_vz.toStdVector();
            int result = analizer.runAnalyzer(time_temp, est_px_temp, des_px_temp, response_time_temp, response_px_temp);
            response_px = stdVector2qVector(response_px_temp);
            result = analizer.runAnalyzer(time_temp, est_py_temp, des_py_temp, response_time_temp, response_py_temp);
            response_py = stdVector2qVector(response_py_temp);
            result = analizer.runAnalyzer(time_temp, est_pz_temp, des_pz_temp, response_time_temp, response_pz_temp);
            response_pz = stdVector2qVector(response_pz_temp);
            result = analizer.runAnalyzer(time_temp, est_vx_temp, des_vx_temp, response_time_temp, response_vx_temp);
            response_vx = stdVector2qVector(response_vx_temp);
            result = analizer.runAnalyzer(time_temp, est_vy_temp, des_vy_temp, response_time_temp, response_vy_temp);
            response_vy = stdVector2qVector(response_vy_temp);
            result = analizer.runAnalyzer(time_temp, est_vz_temp, des_vz_temp, response_time_temp, response_vz_temp);
            response_vz = stdVector2qVector(response_vz_temp);
            response_time = stdVector2qVector(response_time_temp);
            response_ref.clear();
            for (size_t i = 0; i < response_time.size(); i++)
                response_ref.push_back(1.0);
        }
        all_data = {est_px, est_py, est_pz, est_vx, est_vy, est_vz, est_ax, est_ay, est_az, est_yaw,
                    des_px, des_py, des_pz, des_vx, des_vy, des_vz, des_ax, des_ay, des_az,
                    err_px, err_py, err_pz, err_vx, err_vy, err_vz, err_ax, err_ay, err_az,
                    response_px, response_py, response_pz, response_vx, response_vy, response_vz};

        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}