#ifndef STATS_H
#define STATS_H

#include<QtCharts>
#include<QChartView>
#include<QPieSeries>
#include<QPieSlice>


#include <QWidget>

namespace Ui {
class stats;
}

class stats : public QWidget
{
    Q_OBJECT

public:
    explicit stats(QWidget *parent = nullptr);
    void make();
    ~stats();

private:
    Ui::stats *ui;
};

#endif // STATS_H
