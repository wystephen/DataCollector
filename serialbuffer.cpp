#include "serialbuffer.h"

SerialBuffer::SerialBuffer() {}

bool SerialBuffer::setSerialPort(QString port_name, int bund_rate) {
  serial_port_.setPortName(port_name);
  serial_port_.setBaudRate(bund_rate);

  serial_port_.open(QIODevice::ReadOnly);

  if (serial_port_.isOpen()) {
    serial_port_.setDataTerminalReady(true);
  }

  return serial_port_.isOpen();
}

void SerialBuffer::run() {
  running_flag_ = 1;
  char line_buffer[1000];
  while (running_flag_>0) {
    if (running_flag_ == 0) {
      serial_port_.close();
      return;
    }

    if (serial_port_.isOpen()) {
      while (serial_port_.canReadLine()) {
        qint64 len = serial_port_.readLine(line_buffer, 1000);
        //        std::cout << "len:" << len << std::endl;
        if (len > 0 && len < 1000) {
          QString line_str(line_buffer);
          if (out_flag_ == 1) {

            auto filter_func = [](QString raw_str) -> QString {
              auto sub_strs = raw_str.split(" ");
              QString return_str(
                  sub_strs[0].mid(1, sub_strs[0].size() - 2) + "," +
                  QString::number(get_time_now(), 'g', 20) + ",{" +
                  sub_strs[4].right(8) + ":"                      // mac
                  + sub_strs[5] + ","                             // range
                  + sub_strs[6].remove("\r").remove("\n") + ",}"  // intense
              );
              return return_str;
            };

            auto sub_strs = line_str.split(" ");
            if (sub_strs[2] == "@R" && sub_strs[3] != "F1") {
              QString processed_str = filter_func(line_str);
              out_stream << processed_str.toStdString() << std::endl;
              emit newUWB(processed_str);
            }
          }

          emit newUWB(line_str);
        }
      }
      msleep(10);

    } else {
      std::cout << "serial port not openned" << std::endl;
    }
  }

  std::cout << "Stop thread" << std::endl;
  msleep(1000);
  if(serial_port_.isOpen())
      serial_port_.close();

//  this->exit();

}

bool SerialBuffer::startWrite(std::string file_str) {
  out_stream.open(file_str + "HEAD_UWB.data");
  if (out_stream.is_open()) {
    out_flag_ = 1;
    return true;
  } else {
    return false;
  }
}

bool SerialBuffer::stopWrite() {
  out_stream.flush();
  out_flag_ = 0;
  out_stream.close();
  return true;
}
