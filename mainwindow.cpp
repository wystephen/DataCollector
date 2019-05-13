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
//    if(!serial_port_.isOpen()) {
        //TODO:Open serial port
//    }
}



void MainWindow::on_pushButton_2_clicked()
{
//    if(serial_port_.isOpen()){
        // TODO: close
//    }
}

void MainWindow::on_actionMYNTD_triggered()
{

}
