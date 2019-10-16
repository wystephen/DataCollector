#include "jy901reader.h"

JY901Reader::JY901Reader(){}


bool JY901Reader::setSerialPort(QString port_name, int band_rate){
    serial_port_.setPortName(port_name);
    serial_port_.setBaudRate(band_rate);

    serial_port_.open(QIODevice::ReadOnly);

    if(serial_port_.isOpen()){
        serial_port_.setDataTerminalReady(true);
    }
    return serial_port_.isOpen();
}

