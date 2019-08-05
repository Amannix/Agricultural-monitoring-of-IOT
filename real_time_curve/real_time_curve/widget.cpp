#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    initChart();
    addLiner("s",splineSeries);
    createSock();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    ui->widget->setLayout(layout);
    timerId = startTimer(1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *event) {
    // 产生一个数据，模拟不停的接收到新数据
    if (event->timerId() == timerId) {
        cnt++;
        int newData = qrand() % (maxY + 1);
        dataReceived(newData);
    }
}

void Widget::dataReceived(int value) {
    data.push_back(value);

    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        QDateTime temp_AddTimePos(QDate(2019, 1, 1), QTime(0, 0, 0).addSecs(cnt));//生成一个坐标轴时间QDateTime
        if (temp_AddTimePos >= *end){
            *start = start->addSecs(1);
            *end = end->addSecs(1);
            chart->axisX()->setRange(*start,*end);
        }
        splineSeries->append(temp_AddTimePos.toMSecsSinceEpoch(), value);
    }
}

void Widget::addLiner(QString name,QSplineSeries *splineSeries)
{
    chart->addSeries(splineSeries);
    splineSeries->setName(name);
    splineSeries->setUseOpenGL(true);//使用openGL加速
    //splineSeries->setColor(QColor(255,255,0));设置颜色
    chartView->chart()->setAxisX(axisx, splineSeries);//图表添加折线
    chartView->chart()->setAxisY(axisy, splineSeries);
}

void Widget::initChart()
{
    num = 0;
    cnt = 0;
    ui->setupUi(this);
    maxSize = 31; // 只存储最新的 31 个数据
    maxX = 300;
    maxY = 100;
    splineSeries = new QSplineSeries();//折线

    start = new QDateTime(QDate(2019,1,1),QTime(0,0,0));
    end = new QDateTime(QDate(2019,1,1),QTime(0,0,10));

    chart = new QChart();
    chart->legend()->setAlignment(Qt::AlignRight);//隐藏图例
    chart->setTitle("实时动态曲线");
    chart->createDefaultAxes();

    chartView = new QChartView(chart);

    axisx = new QDateTimeAxis();//创建坐标轴
    axisx->setGridLineVisible(false);//隐藏网格
    axisx->setTitleText("time");
    //axisx->setTickCount(11);
    axisx->setFormat("mm:ss");//设置时间显示格式
    axisx->setRange(*start,*end);//设置坐标轴时间范围

    axisy = new QValueAxis();//创建Y坐标轴
    axisy->setMinorTickCount(5);
    axisy->setTitleText("Value");
    axisy->setRange(0, maxY);//设置Y轴范围

    chartView->setRenderHint(QPainter::Antialiasing);//抗锯齿

    qDebug() << "debug\n";
}

void Widget::createSock()
{
    serv = new QTcpServer(this);
    if (serv->listen(QHostAddress::Any,8888)){
        qDebug() << "list OK\n";
    }
    connect(serv,&QTcpServer::newConnection,this,[=]{
        qDebug() << "new connect" << endl;
        clnt = serv->nextPendingConnection();
        connect(clnt, &QTcpSocket::readyRead, this, [=]{
            qDebug() << "newdata:";
            QByteArray data = clnt->readAll();
            QString str(data);
            qDebug() << str << endl;
        });
    });
}
