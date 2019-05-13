#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <thread>

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

    void on_comboBox_activated(const QString &arg1);

    void on_PortCombo_highlighted(const QString &arg1);

    void on_PortCombo_activated(int index);

    void on_comboBox_band_currentIndexChanged(const QString &arg1);

    void on_comboxBox_portName_highlighted(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_comboxBox_portName_currentIndexChanged(const QString &arg1);

    void on_btn_start_serial_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSerialPort serial_port_;
    QSerialPortInfo serial_info_;
};

#endif // MAINWINDOW_H
