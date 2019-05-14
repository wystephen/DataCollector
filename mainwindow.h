#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QHBoxLayout>
#include <QInputDialog>
#include <QPainter>
#include <QTimer>

#include <QMessageBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <thread>

#include <mynteyed/camera.h>
#include <mynteyed/utils.h>

MYNTEYE_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionMYNTD_triggered();


    void on_comboBox_band_currentIndexChanged(const QString &arg1);

//    void on_comboxBox_portName_highlighted(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_comboxBox_portName_currentIndexChanged(const QString &arg1);

    void on_btn_start_serial_clicked(bool checked);

    void handleReadyRead();

    void on_btn_stop_serial_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial_port_;
    QSerialPortInfo serial_info_;

    MYNTEYE_NAMESPACE::Camera mynt_cam_;


    bool selectDevice(DeviceInfo *dev_info);
};

#endif // MAINWINDOW_H
