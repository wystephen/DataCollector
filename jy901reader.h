#ifndef JY901READER_H
#define JY901READER_H

#include <QObject>
#include <QMessageBox>
#include <QThread>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <fstream>
#include <iostream>

#include <datasetwriter.h>

class JY901Reader : public QThread
{
    Q_OBJECT
public:
    explicit JY901Reader();

    bool setSerialPort(QString port_name, int band_rate);

    void stopThread();

    bool startWrite(std::string file_str);

    bool stopWrite();

    bool isValid() {return serial_port_.isOpen();}
protected:
    void run() Q_DECL_OVERRIDE;

private:
    std::ofstream out_stream;

    QAtomicInt out_flag_ = 0;
    QAtomicInt running_flag_ = 0;

    QSerialPort serial_port_;
    QSerialPortInfo serial_info_;


signals:
    void newIMU(QString new_str);


public slots:
};

#endif // JY901READER_H
