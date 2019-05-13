#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
//    serial_port_.close();
    delete ui;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString str = arg1;

}

void MainWindow::on_pushButton_clicked()
{
}



void MainWindow::on_pushButton_2_clicked()
{
}

void MainWindow::on_actionMYNTD_triggered()
{

}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
//    ui->PortCombo->addItem("defult1");
}

void MainWindow::on_PortCombo_highlighted(const QString &arg1)
{
//    ui->PortCombo->addItem("defult2");

}

void MainWindow::on_PortCombo_activated(int index)
{
//    ui->PortCombo->addItem("defult3");

}

void MainWindow::on_comboBox_band_currentIndexChanged(const QString &arg1)
{
    QString baud_rate_str = arg1;
    serial_port_.setBaudRate(baud_rate_str.toInt());
}

void MainWindow::on_comboxBox_portName_highlighted(const QString &arg1)
{


}

void MainWindow::on_pushButton_3_clicked()
{
    ui->comboxBox_portName->clear();
    QList<QSerialPortInfo> port_info_list;
    port_info_list = QSerialPortInfo::availablePorts();
    if(port_info_list.size()>0){
        for(auto port:port_info_list){
            ui->comboxBox_portName->addItem(port.portName());
        }

    }
}

void MainWindow::on_comboxBox_portName_currentIndexChanged(const QString &arg1)
{
   serial_port_.setPortName(arg1);
}

void MainWindow::on_btn_start_serial_clicked(bool checked)
{
    if(!serial_port_.isOpen()) {
        serial_port_.open(QIODevice::ReadOnly);
        QMessageBox::information(NULL,"title", "openning serial port");
        if(serial_port_.isOpen()){
            QMessageBox::information(NULL,"title", "opened serial port");
        }
    }
}
