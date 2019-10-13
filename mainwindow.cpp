#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <mynteyed/utils.h>

MYNTEYE_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->btn_stop_serial->setEnabled(false);
  setupDrawImage();
}

MainWindow::~MainWindow() {
  //    serial_port_.close();
  delete ui;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1) {
  QString str = arg1;
}

void MainWindow::on_actionMYNTD_triggered() {

  mynt_reader_ = new MYNTReader();
  mynt_reader_->setDisplayWidth(left_label_->width());

  // connect display signal.
  connect(mynt_reader_, SIGNAL(newLeft(QImage )), this,
          SLOT(updateLeft(QImage )));

  connect(mynt_reader_,
          SIGNAL(newRight(QImage)),
          this,
          SLOT(updateRight(QImage)));

  connect(mynt_reader_,
          SIGNAL(newDepth(QImage)),
          this,
          SLOT(updateDepth(QImage)));

  connect(mynt_reader_,
          SIGNAL(newIMU(double,double,double,
                        double,double,double,
                        double)),
          this,
          SLOT(drawImuInfo(double,double,double,
                           double,double,double,
                           double))
          );

  mynt_reader_->start();
}

/**
 * @brief MainWindow::updateLeft
 * @param img
 */
void MainWindow::updateLeft(QImage img) {
  left_label_->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::updateRight(QImage img){
    right_label_->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::updateDepth(QImage img){
    depth_label_->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_comboBox_band_currentIndexChanged(const QString &arg1) {
  QString baud_rate_str = arg1;
//  serial_port_.setBaudRate(baud_rate_str.toInt());
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
//    port_name_ = arg1;
}

/**
 * @brief MainWindow::on_btn_start_serial_clicked
 * Start serial port.
 * @param checked
 */
void MainWindow::on_btn_start_serial_clicked(bool checked) {


    if(serial_reader_==nullptr){
        serial_reader_ = new SerialBuffer();
        serial_reader_->setSerialPort(
            ui->comboxBox_portName->currentText(),
            ui->comboBox_band->currentText().toInt());
        connect(serial_reader_,
                SIGNAL(newUWB(QString)),
                this,
                SLOT(showUWB(QString)));


        serial_reader_->start();
         ui->btn_start_serial->setEnabled(false);
      ui->btn_stop_serial->setEnabled(true);

    }
}

void MainWindow::on_btn_stop_serial_clicked() {
  ui->btn_start_serial->setEnabled(true);
  ui->btn_stop_serial->setEnabled(false);
  serial_reader_->stopThread();
  while(serial_reader_->isRunning()){}
  serial_reader_->exit();
  delete serial_reader_;
  serial_reader_ = nullptr;

}

void MainWindow::showUWB(QString str){
//    ui->text_browser->append(QString::fromStdString(str));
    ui->text_browser->append(str);
//    std::cout << "get signal" << str.toStdString() << std::endl;
}

/**
 * @brief MainWindow::handleReadyRead
 * Read one line of UWB observation. And process it.
 */
void MainWindow::handleReadyRead() {
  std::cout << "start handle ready read" << std::endl;
  char data_buf[1000]; // = new char[1000];
  while (serial_port_.canReadLine()) {
    //        memset(data_b/*u*/f,10000,sizeof(char)*10000);
    auto len = serial_port_.readLine(data_buf, 1000);
    if (len > 0 && len < 1000) {
      QString line_data(data_buf);
      ui->text_browser->append(line_data);

    } else {
      std::cout << "some error happend" << std::endl;
      break;
    }
  }
  //    delete []data_buf;
  serial_port_.clearError();
  std::cout << "end handle ready read" << std::endl;
}
/**
 * @brief MainWindow::setupDrawImage
 * @return
 */
bool MainWindow::setupDrawImage() {
  QWidget *widget = ui->camWidget;
  QGridLayout *layout = new QGridLayout(widget);

  layout->setMargin(0);
  layout->setSpacing(0);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  int width = widget->width() / 3;
  int height = widget->height();

  left_label_ = new QLabel(widget);
  left_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  left_label_->setFixedSize(width, height);
  layout->addWidget(left_label_, 0, 0);

  right_label_ = new QLabel(widget);
  right_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  right_label_->setFixedSize(width, height);
  layout->addWidget(right_label_, 0, 1);
  depth_label_ = new QLabel(widget);
  depth_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  depth_label_->setFixedSize(width, height);
  layout->addWidget(depth_label_, 0, 2);

  return true;
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

    if(mynt_reader_){
         mynt_reader_->startWrite((save_dir+"/"
                              +sub_string+"/").toStdString());

    }

    // if serial port available. start saving , disable stop button.
    if(serial_reader_){
        if(serial_reader_->startWrite((save_dir+"/"+sub_string+"/").toStdString()))
        {
            std::cout << "started writing in serial buffer" << std::endl;
            ui->btn_stop_serial->setEnabled(false);
        }else{
            QMessageBox::warning(nullptr,
                                 "start serial record error",
                                 "failed to create file in serial recorder thread");
        }
    }

      saving_flag = true;
      ui->save_dir_label->setLineWidth(3);
      ui->save_dir_label->setText("SAVING TO" + save_dir + "/" + sub_string);
  }
}

void MainWindow::on_actionStop_Record_triggered() {
  if (saving_flag) {
    saving_flag = false;
    // close file handle.
    ui->save_dir_label->setLineWidth(1);
    ui->save_dir_label->setText("STOP SAVING");

    if(mynt_reader_){
        mynt_reader_->stopWrite();
    }

    if(serial_reader_){
        serial_reader_->stopWrite();
        ui->btn_stop_serial->setEnabled(true);
    }
  }
}

void MainWindow::on_btn_start_imu_clicked()
{

}
