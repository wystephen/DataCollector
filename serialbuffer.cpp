#include "serialbuffer.h"

SerialBuffer::SerialBuffer(){

}

bool SerialBuffer::setSerialPort(QString port_name,
                                 int bund_rate)
{
    serial_port_.setPortName(port_name);
    serial_port_.setBaudRate(bund_rate);

    serial_port_.open(QIODevice::ReadOnly);

    if(serial_port_.isOpen()){
        serial_port_.setDataTerminalReady(true);
    }

    return serial_port_.isOpen();

}

void SerialBuffer::run(){
    running_flag_  = 1;
    char line_buffer[1000];
    while(true){
        if(running_flag_==0){
            serial_port_.close();
            return;
        }

        if(serial_port_.isOpen()){
            while(serial_port_.canReadLine()){

                qint64 len =
                    serial_port_.readLine(line_buffer,1000);
                std::cout<< "len:" << len << std::endl;
                if(len >0 && len < 1000){

                    QString line_str(line_buffer);
                    if(out_flag_==1){
                        out_stream << line_str.toStdString() << std::endl;
//                        out_stream << "empty" << std::endl;
                        // TODO: pre process data.

                    }

                    emit newUWB(line_str);
                }


            }
            msleep(10);


        }else{
            std::cout << "serial port not openned" << std::endl;
        }
    }
}

bool SerialBuffer::startWrite(std::string file_str){
    out_stream.open(file_str+"HEAD_UWB.data");
    if(out_stream.is_open()){

    out_flag_ = 1;
    return true;
    }else{
        return false;
    }
}

bool SerialBuffer::stopWrite(){
    out_stream.flush();
    out_flag_ = 0;
    out_stream.close();
    return true;

}
