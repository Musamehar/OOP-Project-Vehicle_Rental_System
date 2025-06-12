#include "ReportsPage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include "qcustomplot.h"

ReportsPage::ReportsPage(QWidget* parent)
    : QWidget(parent) {
    barChart = new QCustomPlot(this);
    lineChart = new QCustomPlot(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(barChart);
    layout->addWidget(lineChart);

    setupBarChart();
    setupLineChart();
}

void ReportsPage::setupBarChart() {
    QVector<QString> labels;
    QVector<double> values;
    QVector<double> ticks;

    QSqlQuery query = DatabaseManager::getRevenuePerVehicle();
    int i = 0;
    while (query.next()) {
        labels.append(query.value(0).toString());
        values.append(query.value(1).toDouble());
        ticks.append(i++);
    }

    QCPBars* bars = new QCPBars(barChart->xAxis, barChart->yAxis);
    bars->setData(ticks, values);

    QSharedPointer<QCPAxisTickerText> barTicker(new QCPAxisTickerText);
    for (int j = 0; j < labels.size(); ++j)
        barTicker->addTick(j, labels[j]);

    barChart->xAxis->setTicker(barTicker);
    barChart->xAxis->setTickLabelRotation(60);
    barChart->yAxis->setLabel("Revenue ($)");
    bars->setName("Vehicle Revenue");

    barChart->legend->setVisible(true);
    barChart->replot();
}

void ReportsPage::setupLineChart() {
    QVector<double> x, y;
    QVector<QString> months;

    QSqlQuery query = DatabaseManager::getMonthlyProfitLoss();
    int i = 0;
    while (query.next()) {
        months.append(query.value(0).toString());
        x.append(i);
        y.append(query.value(1).toDouble());
        ++i;
    }

    lineChart->addGraph();
    lineChart->graph(0)->setData(x, y);

    QSharedPointer<QCPAxisTickerText> lineTicker(new QCPAxisTickerText);
    for (int j = 0; j < months.size(); ++j)
        lineTicker->addTick(j, months[j]);

    lineChart->xAxis->setTicker(lineTicker);
    lineChart->xAxis->setTickLabelRotation(45);
    lineChart->yAxis->setLabel("Profit ($)");

    lineChart->graph(0)->setName("Monthly Profit");
    lineChart->legend->setVisible(true);
    lineChart->replot();
}
