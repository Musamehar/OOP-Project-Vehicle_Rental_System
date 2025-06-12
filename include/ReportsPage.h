#ifndef REPORTSPAGE_H
#define REPORTSPAGE_H

#include <QWidget>

class QCustomPlot;

class ReportsPage : public QWidget {
    Q_OBJECT

    QCustomPlot* barChart;
    QCustomPlot* lineChart;

    void setupBarChart();
    void setupLineChart();

public:
    explicit ReportsPage(QWidget* parent = nullptr);
};

#endif
