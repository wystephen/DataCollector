#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <mynteyed/utils.h>

MYNTEYE_USE_NAMESPACE

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
}



void MainWindow::on_pushButton_2_clicked()
{
}

void MainWindow::on_actionMYNTD_triggered()
{
    DeviceInfo dev_info;

    bool cam_ok = selectDevice(&dev_info);
    if(cam_ok){
        OpenParams cam_params(dev_info.index);

        cam_params.framerate=30;
        cam_params.stream_mode = StreamMode::STREAM_2560x720;

        cam_params.color_mode = ColorMode::COLOR_RAW;
//        cam_params.depth_mode = DepthMode::DEPTH_RAW;
        cam_params.depth_mode = DepthMode::DEPTH_COLORFUL;

//        cam_params.ir_depth_only = true;

        cam_params.ir_intensity = 0;//close IR

        mynt_cam_.EnableMotionDatas();
        mynt_cam_.Open(cam_params);

        if(mynt_cam_.IsOpened()){

            // save sensor parameters.

            //TODO: here
            // start display
            setupDrawImage();

        }

    }

}

void MainWindow::on_comboBox_band_currentIndexChanged(const QString &arg1)
{
    QString baud_rate_str = arg1;
    serial_port_.setBaudRate(baud_rate_str.toInt());
}

/**
 * @brief MainWindow::on_pushButton_3_clicked
 * refresh button, refresh serial port device name in comboBox.
 */
void MainWindow::on_pushButton_3_clicked()
{
    ui->comboxBox_portName->clear();
    QList<QSerialPortInfo> port_info_list;
    port_info_list = QSerialPortInfo::availablePorts();
    if(port_info_list.size()>0){
        for(auto port:port_info_list){
            ui->comboxBox_portName->addItem(port.portName());
        }

    }
}

/**
 * @brief MainWindow::on_comboxBox_portName_currentIndexChanged
 * set port name.
 * @param arg1 name of serial port device.
 */
void MainWindow::on_comboxBox_portName_currentIndexChanged(const QString &arg1)
{
   serial_port_.setPortName(arg1);
}

/**
 * @brief MainWindow::on_btn_start_serial_clicked
 * Start serial port.
 * @param checked
 */
void MainWindow::on_btn_start_serial_clicked(bool checked)
{
    if(!serial_port_.isOpen()) {
        serial_port_.open(QIODevice::ReadOnly);
//        QMessageBox::information(NULL,"title", "openning serial port");
        if(serial_port_.isOpen()){
//            QMessageBox::information(NULL,"title", "opened serial port");
            connect(&serial_port_,&QSerialPort::readyRead,
                    this,&MainWindow::handleReadyRead);
            ui->btn_start_serial->setEnabled(false);
            ui->btn_stop_serial->setEnabled(true);
        }else{
            QMessageBox::information(
                        nullptr,
                        "SerialPortError",
                        "UWB serial port open failed");
        }
    }
}

void MainWindow::on_btn_stop_serial_clicked()
{
    serial_port_.close();
    ui->btn_start_serial->setEnabled(true);
}

/**
 * @brief MainWindow::handleReadyRead
 * Read one line of UWB observation. And process it.
 */
void MainWindow::handleReadyRead(){
    char* data_buf = new char[1000];
    serial_port_.readLine(data_buf,1000);
    QString line_data(data_buf);
    ui->text_browser->append(line_data);
}


bool MainWindow::selectDevice(DeviceInfo *dev_info){
   std::vector<DeviceInfo> dev_infos = mynt_cam_.GetDeviceInfos();
   std::size_t n = dev_infos.size();
   if(n<=0){
       QMessageBox::warning(
                   NULL,
                   tr("ERROR"),
                   tr("No MYNT@ EYE Depth Camera detected"),
                      QMessageBox::Ok
                      );
       return false;
   }else{
       if (n == 1) {
              *dev_info = dev_infos[0];
              return true;
          } else {  // > 1
              QStringList items;
              for (auto &&info : dev_infos) {
                  items << QString("%1 | %2").arg(info.index).arg(info.name.c_str());
              }

              bool ok;
              QString item = QInputDialog::getItem(this,
                  tr("Select"), tr("MYNT® EYE cameras"), items, 0, false, &ok);
              if (ok) {
                  QStringList parts = item.split("|");
                  if (parts.length() > 0) {
                      QString index =  parts[0].trimmed();
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

bool MainWindow::setupDrawImage(){
    left_enabled_ = mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_LEFT_COLOR);
    right_enabled_ = mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_RIGHT_COLOR);
    depth_enabled_ = mynt_cam_.IsStreamDataEnabled(ImageType::IMAGE_DEPTH);

    QWidget *widget = ui->camWidget;
    QGridLayout *layout = new QGridLayout(widget);

    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    int width = widget->width() /4;
    int height = widget->height();

    if (left_enabled_) {
        left_label_ = new QLabel(widget);
        left_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        left_label_->setFixedSize(width, height);
        layout->addWidget(left_label_, 0, 0);
    }
    if (right_enabled_) {
        right_label_= new QLabel(widget);
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
    connect(cam_timer_, SIGNAL(timeout()),
            this, SLOT(processStream()));
    cam_timer_->start(1000/200);
}


void MainWindow::processStream(){
    if(left_enabled_){
        auto && left_img = mynt_cam_.GetStreamData(ImageType::IMAGE_LEFT_COLOR);
        if(left_img.img){
            auto &&img = left_img.img->To(ImageFormat::COLOR_RGB);
            QImage image(img->data(),img->width(),img->height(),
                         QImage::Format_RGB888);
            QImage small_image = image.scaledToWidth(left_label_->width());
            left_label_->setPixmap(QPixmap::fromImage(small_image));
        }
    }
}
