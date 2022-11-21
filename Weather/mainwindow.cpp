#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(weatherThread, SIGNAL(InfoObtained(Weather)), this, SLOT(onInfoObtained(Weather)));
    QObject::connect(weatherThread, SIGNAL(ChangeStatus(QString)), this, SLOT(onChangeStatus(QString)));
    QObject::connect(weatherThread, SIGNAL(EventHappened(QString)), this, SLOT(onEventHappened(QString)));

    QStringList headerLabels;
    headerLabels << "Температура" << "Відчувається" << "Тип" << "Місто" << "Час отримання";
    ui->weatherTable->setHorizontalHeaderLabels(headerLabels);
    ui->weatherTable->setColumnWidth(0, 110);
    ui->weatherTable->setColumnWidth(1, 110);
    ui->weatherTable->setColumnWidth(2, 110);
    ui->weatherTable->setColumnWidth(3, 135);
    ui->weatherTable->setColumnWidth(4, 218);

    ui->weatherTable->setTextElideMode(Qt::ElideNone);

    onChangeStatus("Не під'єднано");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getButton_clicked()
{
    weatherThread->start();
}

void MainWindow::on_stopButton_clicked()
{
    if(weatherThread->isActive()) {
        weatherThread->kill();
        onChangeStatus("Від'єднується");
    }
}

void MainWindow::onInfoObtained(Weather weather){
    if( !weather.getWeatherType().size() || !weather.getCity().size() || !weather.getDate().size() )
        return;
    ui->weatherTable->insertRow(ui->weatherTable->rowCount());
    for(int k=0; k!=ui->weatherTable->columnCount(); ++k){
        QTableWidgetItem *itm = new QTableWidgetItem();
        ui->weatherTable->setItem(ui->weatherTable->rowCount()-1, k, itm);
    }

    ui->weatherTable->item(ui->weatherTable->rowCount()-1, 0)->setText(QString::number(weather.getTemperature()));
    ui->weatherTable->item(ui->weatherTable->rowCount()-1, 1)->setText(QString::number(weather.getFeelsLike()));
    ui->weatherTable->item(ui->weatherTable->rowCount()-1, 2)->setText(weather.getWeatherType());
    ui->weatherTable->item(ui->weatherTable->rowCount()-1, 3)->setText(weather.getCity());
    ui->weatherTable->item(ui->weatherTable->rowCount()-1, 4)->setText(weather.getDate());
}

void MainWindow::onChangeStatus(QString status){
    ui->statusLabel->setText(status);
}

void MainWindow::onEventHappened(QString msg){
    QMessageBox::information(this, "Увага!", msg);
}
