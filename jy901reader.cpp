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

void JY901Reader::run(){
    running_flag_ = 1;
    char line_buffer[20];
    while(running_flag_>0){
        if(running_flag_==0){
            serial_port_.close();
        }
        if(serial_port_.isOpen()){

        }else{
           std::cout <<__FILE__ << ":"
                    << __FUNCTION__ << ":"
                    << __LINE__ <<":"
                    << "serial port not openned" << std::endl;
        }
    }
    serial_port_.close();

}
