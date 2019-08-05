#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QtSvg/QSvgWidget>
#include <QtSvg/QSvgRenderer>
#include <QMouseEvent>
#include <QQueue>
#include <QString>
#include <QtCharts>
#include <QTcpServer>
#include <QTcpSocket>

using namespace QtCharts;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void addLiner(QString name,QSplineSeries *splineSeries);
    void initChart();
    void createSock();

private slots:
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    int num;
    Ui::Widget *ui;
    void dataReceived(int value);
    int timerId;
    int maxSize;  // data 最多存储 maxSize 个元素
    int maxX;
    int maxY;
    int cnt;
    QQueue<double> data; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;
    int kx;
    qint64 current_time_total_ms;
    QDateTime *start,*end;
    QValueAxis *axisy;
    QDateTimeAxis *axisx;
    QTcpServer *serv;
    QTcpSocket *clnt;

};
#endif // WIDGET_H
