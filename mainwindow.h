#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QHBoxLayout>
#include <QInputDialog>
#include <QPainter>
#include <QTimer>

#include <QMessageBox>

#include <QFileDialog>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <thread>

#include <mynteyed/camera.h>
#include <mynteyed/utils.h>

#include <datasetwriter.h>

MYNTEYE_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_comboBox_currentTextChanged(const QString &arg1);

//  void on_pushButton_clicked();

//  void on_pushButton_2_clicked();

  void on_actionMYNTD_triggered();

//  void on_comboBox_band_currentIndexChanged(const QString &arg1);

  //    void on_comboxBox_portName_highlighted(const QString &arg1);

  void on_pushButton_3_clicked();

  void on_comboxBox_portName_currentIndexChanged(const QString &arg1);

  void on_btn_start_serial_clicked(bool checked);

  void handleReadyRead();

  void on_btn_stop_serial_clicked();

  void checkSerialStatu();

  void processStream();

  void on_actionStart_Record_triggered();

  void on_actionStop_Record_triggered();

  private:
  Ui::MainWindow *ui;
  QSerialPort serial_port_;
  QSerialPortInfo serial_info_;

  QTimer *serial_timer_ = nullptr;

  /// MYNT CAM
  MYNTEYE_NAMESPACE::Camera mynt_cam_;

  bool left_enabled_ = false;
  bool right_enabled_ = false;
  bool depth_enabled_ = false;

  QLabel *left_label_;
  QLabel *right_label_;
  QLabel *depth_label_;
  QLabel *info_label_;

  QTimer *cam_timer_;


  /// FILE SAVE
  QString save_dir = "";
  bool saving_flag = false;
  DatasetWriter *writer_ptr_;

  bool selectDevice(DeviceInfo *dev_info);

  bool setupDrawImage();

  bool drawImuInfo(double acc_x, double acc_y, double acc_z, double gyr_x,
                   double gyr_y, double gyr_z, double temp);
};

#endif // MAINWINDOW_H
