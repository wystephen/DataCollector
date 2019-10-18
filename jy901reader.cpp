#include "jy901reader.h"

JY901Reader::JY901Reader(){}


bool JY901Reader::setSerialPort(QString port_name, int band_rate){
    serial_port_.setPortName(port_name);
    serial_port_.setBaudRate(band_rate);

    serial_port_.open(QIODevice::ReadOnly);

    if(serial_port_.isOpen()){
        serial_port_.setDataTerminalReady(true);
    }else{
        QMessageBox::warning(nullptr,
                             "cannot open serial "+ port_name +" at band_rate" +
                             QString::fromStdString(std::to_string(band_rate)),"error");
    }
    return serial_port_.isOpen();
}

void JY901Reader::run(){
    running_flag_ = 1;
    constexpr int buf_size = 30;
    char line_buffer[buf_size];
    int buf_index = 0;

    while(running_flag_>0){
        if(serial_port_.isOpen()){
//            qint64 len = serial_port_.readData(line_buffer+buf_index, buf_size-buf_index);)
            qint64 len = serial_port_.read(line_buffer+buf_index,buf_size-buf_index);
            if(len>0){
                printf("accept:");
                for(int i=0;i<len;++i){
                    printf("%x ",line_buffer[i]);
                }
                printf("\n");

                if(out_flag_==1){


                }
            }else{
                msleep(10);
            }



        }else{
           std::cout <<__FILE__ << ":"
                    << __FUNCTION__ << ":"
                    << __LINE__ <<":"
                    << "serial port not openned" << std::endl;
        }
    }
    msleep(1000);
    if(serial_port_.isOpen()){
        serial_port_.close();
    }


}


bool JY901Reader::startWrite(std::string file_str){
    out_stream.open(file_str+"HEAD_IMU.data");
    if(out_stream.is_open()){
        out_flag_ = 1;
        return true;
    }else{
        return false;
    }
}

bool JY901Reader::stopWrite(){
    out_stream.flush();
    out_flag_ = 0;
    out_stream.close();
    return true;
}
