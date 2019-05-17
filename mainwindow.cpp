#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <mynteyed/utils.h>

MYNTEYE_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->btn_stop_serial->setEnabled(false);
}

MainWindow::~MainWindow() {
  //    serial_port_.close();
  delete ui;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1) {
  QString str = arg1;
}

//void MainWindow::on_pushButton_clicked() {}

//void MainWindow::on_pushButton_2_clicked() {}

void MainWindow::on_actionMYNTD_triggered() {
  DeviceInfo dev_info;

  bool cam_ok = selectDevice(&dev_info);
  if (cam_ok) {
    OpenParams cam_params(dev_info.index);

    cam_params.framerate = 20;
    cam_params.stream_mode = StreamMode::STREAM_2560x720;

    cam_params.color_mode = ColorMode::COLOR_RAW;
    //        cam_params.depth_mode = DepthMode::DEPTH_GRAY;
    cam_params.depth_mode = DepthMode::DEPTH_COLORFUL;

    cam_params.ir_depth_only = false;

    cam_params.ir_intensity = 0; // close IR

    mynt_cam_.EnableImageInfo(true);
    mynt_cam_.EnableMotionDatas();
    mynt_cam_.Open(cam_params);

    if (mynt_cam_.IsOpened()) {

      // save sensor parameters.

      // TODO: here
      // start display
      setupDrawImage();
    }
  }
}

void MainWindow::on_comboBox_band_currentIndexChanged(const QString &arg1) {
  QString baud_rate_str = arg1;
  serial_port_.setBaudRate(baud_rate_str.toInt());
}

/**
 * @brief MainWindow::on_pushButton_3_clicked
 * refresh button, refresh serial port device name in comboBox.
 */
void MainWindow::on_pushButton_3_clicked() {
  ui->comboxBox_portName->clear();
  QList<QSerialPortInfo> port_info_list;
  port_info_list = QSerialPortInfo::availablePorts();
  if (port_info_list.size() > 0) {
    for (auto port : port_info_list) {
      ui->comboxBox_portName->addItem(port.portName());
    }
  }
}

/**
 * @brief MainWindow::on_comboxBox_portName_currentIndexChanged
 * set port name.
 * @param arg1 name of serial port device.
 */
void MainWindow::on_comboxBox_portName_currentIndexChanged(
    const QString &arg1) {
  serial_port_.setPortName(arg1);
}

/**
 * @brief MainWindow::on_btn_start_serial_clicked
 * Start serial port.
 * @param checked
 */
void MainWindow::on_btn_start_serial_clicked(bool checked) {
  if (!serial_port_.isOpen()) {
    serial_port_.open(QIODevice::ReadOnly);
    //        QMessageBox::information(NULL,"title", "openning serial port");
    if (serial_port_.isOpen()) {
        serial_port_.setDataTerminalReady(true);
      //            QMessageBox::information(NULL,"title", "opened serial
      //            port");
      connect(&serial_port_, &QSerialPort::readyRead, this,
              &MainWindow::handleReadyRead);
      ui->btn_start_serial->setEnabled(false);
      ui->btn_stop_serial->setEnabled(true);
//      serial_timer_ = new QTimer(this);
      //            serial_timer_->setInterval(100);
//      connect(serial_timer_, SIGNAL(timeout()), this, SLOT(checkSerialStatu()));
//      serial_timer_->start(100);
    } else {
      QMessageBox::information(nullptr, "SerialPortError",
                               "UWB serial port open failed");
    }
  }
}

void MainWindow::checkSerialStatu() {
  if (!(serial_port_.isOpen())) {
    QMessageBox::information(this, "Serial port error",
                             "serial port maybe blocked");
    std::cout << "ERROR of SERIAL PORT" << std::endl;

  } else {
    char *data_buf = new char[1000];
    auto len = serial_port_.readLine(data_buf, 1000);

    if (len > 0) {
      QString line_data(data_buf);
//      ui->text_browser->clear();
      ui->text_browser->append(line_data);
//      ui->text_browser->setText(line_data);
      ui->serial_label->setText(line_data);
      std::cout << "timer block recieved data:" << data_buf << std::endl;
    }
  }
}

void MainWindow::on_btn_stop_serial_clicked() {
  if (serial_port_.isOpen()) {
    serial_port_.close();
  }
  ui->btn_start_serial->setEnabled(true);
  ui->btn_stop_serial->setEnabled(false);
  if (serial_timer_) {
    serial_timer_->stop();
    delete serial_timer_;
    serial_timer_ = nullptr;
  }
}

/**
 * @brief MainWindow::handleReadyRead
 * Read one line of UWB observation. And process it.
 */
void MainWindow::handleReadyRead() {
    std::cout << "start handle ready read" << std::endl;
    char data_buf[1000];// = new char[1000];
    while (serial_port_.canReadLine()) {
//        memset(data_b/*u*/f,10000,sizeof(char)*10000);
      auto len = serial_port_.readLine(data_buf, 1000);
      if (len > 0 && len < 1000) {
        QString line_data(data_buf);
        //            ui->text_browser->clear();
        ui->text_browser->append(line_data);
        //    ui->text_browser->setText(line_data);
        //    ui->serial_label->setText(line_data);
//        std::cout << "recieved data:" << data_buf << std::endl;

      } else {
        std::cout << "some error happend" << std::endl;
        break;
      }
    }
//    delete []data_buf;
    serial_port_.clearError();
    std::cout << "end handle ready read" << std::endl;
}

bool MainWindow::selectDevice(DeviceInfo *dev_info) {
  std::vector<DeviceInfo> dev_infos = mynt_cam_.GetDeviceInfos();
  std::size_t n = dev_infos.size();
  if (n <= 0) {
    QMessageBox::warning(NULL, tr("ERROR"),
                         tr("No MYNT@ EYE Depth Camera detected"),
                         QMessageBox::Ok);

    return false;
  } else {
    if (n == 1) {
      *dev_info = dev_infos[0];
      return true;
    } else { // > 1
      QStringList items;
      for (auto &&info : dev_infos) {
        items << QString("%1 | %2").arg(info.index).arg(info.name.c_str());
      }

      bool ok;
      QString item = QInputDialog::getItem(
          this, tr("Select"), tr("MYNT® EYE cameras"), items, 0, false, &ok);
      if (ok) {
        QStringList parts = item.split("|");
        if (parts.length() > 0) {
          QString index = parts[0].trimmed();
          for (auto &&info : dev_infos) {
            if (QString::number(info.index) == index) {
              *dev_info = info;
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}
/**
 * @brief MainWindow::setupDrawImage
 * @return
 */
bool MainWindow::setupDrawImage() {
  left_enabled_ = mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_LEFT_COLOR);
  right_enabled_ = mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_RIGHT_COLOR);
  depth_enabled_ = mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_DEPTH);

  QWidget *widget = ui->camWidget;
  QGridLayout *layout = new QGridLayout(widget);

  layout->setMargin(0);
  layout->setSpacing(0);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  int width = widget->width() / 3;
  int height = widget->height();

  if (left_enabled_) {
    left_label_ = new QLabel(widget);
    left_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    left_label_->setFixedSize(width, height);
    layout->addWidget(left_label_, 0, 0);
  }
  if (right_enabled_) {
    right_label_ = new QLabel(widget);
    right_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    right_label_->setFixedSize(width, height);
    layout->addWidget(right_label_, 0, 1);
  }
  if (depth_enabled_) {
    depth_label_ = new QLabel(widget);
    depth_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    depth_label_->setFixedSize(width, height);
    layout->addWidget(depth_label_, 0, 2);
  }

  cam_timer_ = new QTimer(this);
  connect(cam_timer_, SIGNAL(timeout()), this, SLOT(processStream()));
  cam_timer_->start(5);
  return true;
}

/**
 * @brief MainWindow::processStream
 * run by timer  (at 100hz).
 * recieve mynt depth camera image and motion data.
 */
void MainWindow::processStream() {
  if (mynt_cam_.HasStreamDataEnabled()) {
//if(mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_LEFT_COLOR)
//&& mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_RIGHT_COLOR)){
    bool left_ok(false), right_ok(false), depth_ok(false);
    auto &&left = mynt_cam_.GetStreamData(ImageType::IMAGE_LEFT_COLOR);
    auto &&right = mynt_cam_.GetStreamData(ImageType::IMAGE_RIGHT_COLOR);
    auto &&depth = mynt_cam_.GetStreamData(ImageType::IMAGE_DEPTH);
    auto &&datas = mynt_cam_.GetMotionDatas();

//    if(!left.img || !right.img
//        || !depth.img){
////        QMessageBox::InformationBox("not read all image");
//        QMessageBox::warning(this,"not readed all image","not read all");


//    }


    auto write_image = [](QImage  img,
                                   QString path_str){
        img.save(path_str);
    };




    if (left_enabled_) {
      if (left.img) {
        left_ok = true;
        auto &&img = left.img->To(ImageFormat::COLOR_RGB);

        QImage image(img->data(), img->width(), img->height(),
                     QImage::Format_RGB888);
        if (writer_ptr_ && writer_ptr_->IsValid() && saving_flag) {
          QString path_str =
              QString::fromStdString(writer_ptr_->RecordLeftImage(
                  left.img_info->timestamp, left.img_info->exposure_time,
                  left.img_info->frame_id));
//          image.save(path_str);
          std::thread left_thread(write_image,image,path_str);
          left_thread.detach();
        }
        if(left.img_info->frame_id%3==0){
            QImage small_image = image.scaledToWidth(left_label_->width());
            left_label_->setPixmap(QPixmap::fromImage(small_image));
        }
      }
    }
    if (right_enabled_) {
      if (right.img) {

        right_ok = true;
        auto &&img = right.img->To(ImageFormat::COLOR_RGB);
        QImage image(img->data(), img->width(), img->height(),
                     QImage::Format_RGB888);

        if (writer_ptr_ && writer_ptr_->IsValid() && saving_flag) {
          QString path_str =
              QString::fromStdString(writer_ptr_->RecordRightImage(
                  right.img_info->timestamp, right.img_info->exposure_time,
                  right.img_info->frame_id));

//          image.save(path_str);
          std::thread right_thread(write_image,image,path_str);
          right_thread.detach();

        }
//        if(right.img_info->frame_id%3==0){
//            QImage small_img = image.scaledToWidth(right_label_->width());
//            right_label_->setPixmap(QPixmap::fromImage(small_img));

//        }
      }
    }
    if (depth_enabled_) {
      if (depth.img) {
        depth_ok = true;
        auto &&img = depth.img->To(ImageFormat::COLOR_RGB);
        QImage image(img->data(), img->width(), img->height(),
                     QImage::Format_RGB888);
        if(depth.img_info->frame_id%3==0){
             QImage small_img = image.scaledToWidth(right_label_->width());
            depth_label_->setPixmap(QPixmap::fromImage(small_img));

        }

      }
    }

    //    if (mynt_cam_.IsMotionDatasEnabled() && left_ok && right_ok) {
    if (!datas.empty()) {
      //      std::cout << "collected imu and gyr data:" << datas.size() <<
      //      std::endl;

      std::shared_ptr<ImuData> accel = nullptr;
      std::shared_ptr<ImuData> gyro = nullptr;
      for (auto &&data : datas) {
        if (!data.imu)
          continue;

        if (data.imu->flag == MYNTEYE_IMU_ACCEL) {
          accel = data.imu;
          if (writer_ptr_ && writer_ptr_->IsValid() && saving_flag) {
            writer_ptr_->RecordAccData(accel->accel[0], accel->accel[1],
                                       accel->accel[2], accel->timestamp);
          }
        } else if (data.imu->flag == MYNTEYE_IMU_GYRO) {
          gyro = data.imu;
          if (writer_ptr_ && writer_ptr_->IsValid() && saving_flag) {
            writer_ptr_->RecordGyrData(gyro->gyro[0], gyro->gyro[1],
                                       gyro->gyro[2], gyro->timestamp);
          }
        } else {
          continue;
        }

        if (accel && gyro) {
          drawImuInfo(accel->accel[0], accel->accel[1], accel->accel[2],
                      gyro->gyro[0], gyro->gyro[1], gyro->gyro[2],
                      accel->timestamp);
        }
        //          break;
      }
      if (accel && gyro) {
      }
    }
    //    }

    if (writer_ptr_ && writer_ptr_->IsValid() && saving_flag) {
      writer_ptr_->Flush();
    }
  }
}

/**
 * @brief MainWindow::drawImuInfo display data in the label.
 * @param acc_x
 * @param acc_y
 * @param acc_z
 * @param gyr_x
 * @param gyr_y
 * @param gyr_z
 * @param temp
 * @return
 */
bool MainWindow::drawImuInfo(double acc_x, double acc_y, double acc_z,
                             double gyr_x, double gyr_y, double gyr_z,
                             double temp) {
  QString s = QString("[acc: %1,%2,%3]\n[gyr:%4,%5,%6]\ntemp:%7")
                  .arg(QString::number(acc_x, 'f', 10))
                  .arg(QString::number(acc_y, 'f', 10))
                  .arg(QString::number(acc_z, 'f', 10))
                  .arg(QString::number(gyr_x, 'f', 10))
                  .arg(QString::number(gyr_y, 'f', 10))
                  .arg(QString::number(gyr_z, 'f', 10))
                  .arg(QString::number(temp, 'f', 20));
  ui->imu_info_label->setText(s);
  return true;
}

void MainWindow::on_actionStart_Record_triggered() {
  QString curPath;
  if (save_dir.isEmpty()) {

    curPath = QCoreApplication::applicationDirPath();
  } else {
    curPath = save_dir;
  }
  QString selectedDir = QFileDialog::getExistingDirectory(
      this, "select a directory", curPath, QFileDialog::ShowDirsOnly);
  if (!selectedDir.isEmpty()) {
    save_dir = selectedDir;
    QDir dir_handle(save_dir);
    QStringList dir_name_list = dir_handle.entryList(QDir::AllDirs);
    // find largest dir satisfy the (00001) format.
    int largest_dir_index = 0;
    QString pattern("[0-9]{5}");
    QRegExp rx(pattern);
    for (auto &name : dir_name_list) {
      if (rx.exactMatch(name) && name.toInt() > largest_dir_index) {
        largest_dir_index = name.toInt();
      }
    }
    largest_dir_index += 1;

    QString sub_string =
        QString("%1").arg(largest_dir_index, 5, 10, QLatin1Char('0'));
    /**
    Create file syste such as:
    00001/
        left/
            *.jpg
            image.csv
        right/
            *.jpg
            image.csv
        Motion.csv
    */
    dir_handle.mkdir(sub_string);
    dir_handle.setPath(save_dir + "/" + sub_string);
    dir_handle.mkdir("right");
    dir_handle.mkdir("left");

    writer_ptr_ =
        new DatasetWriter((save_dir + "/" + sub_string + "/").toStdString());

    if (writer_ptr_->IsValid()) {
      saving_flag = true;
      ui->save_dir_label->setLineWidth(3);
      ui->save_dir_label->setText("SAVING TO" + save_dir + "/" + sub_string);
    } else {
      QMessageBox::warning(nullptr, "Error to saving files",
                           "writer of dataset create failed");
    }
  }
}

void MainWindow::on_actionStop_Record_triggered() {
  if (saving_flag) {
    saving_flag = false;
    // close file handle.
    ui->save_dir_label->setLineWidth(1);
    ui->save_dir_label->setText("STOP SAVING");
    //
    writer_ptr_->Close();
    delete writer_ptr_;
  }
}
