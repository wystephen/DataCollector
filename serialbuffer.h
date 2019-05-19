#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QThread>
#include <QMessageBox>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <fstream>
#include <iostream>




class SerialBuffer :public QThread
{
    Q_OBJECT
public:
    explicit SerialBuffer();


    bool setSerialPort(QString port_name,
                       int bund_rate);

    void stopThread(){
        running_flag_ = 0;
    }

    bool startWrite(std::string file_str);

    bool stopWrite();


    bool isValid(){
        return serial_port_.isOpen();
    }

protected:
    void run() Q_DECL_OVERRIDE;

private:
    std::ofstream out_stream;
    QAtomicInt out_flag_=0;

    QAtomicInt running_flag_ = 0;

    QSerialPort serial_port_;
    QSerialPortInfo serial_info_;

signals:
    //    void newUWB(QString new_str);
    void newUWB(QString new_str);

public slots:


};

#endif // SERIALREADER_H
