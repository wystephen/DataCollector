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

class JY901Reader : public QObject
{
    Q_OBJECT
public:
    explicit JY901Reader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // JY901READER_H
