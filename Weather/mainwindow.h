#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QScrollBar>
#include "weatherthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    WeatherThread* weatherThread = new WeatherThread(this);
    ~MainWindow();

public slots:
    void onInfoObtained(Weather);
    void onChangeStatus(QString);
    void onEventHappened(QString);

private slots:
    void on_getButton_clicked();
    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
