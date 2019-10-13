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

#include <functional>
#include <thread>
//#include <function>

#include <mynteyed/camera.h>
#include <mynteyed/utils.h>

#include <myntreader.h>

#include <datasetwriter.h>

#include <serialbuffer.h>
//#include <serialreader.h>

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

  /**
   * @brief on_actionMYNTD_triggered
   */
  void on_actionMYNTD_triggered();

  void on_comboBox_band_currentIndexChanged(const QString &arg1);

  //    void on_comboxBox_portName_highlighted(const QString &arg1);

  void on_pushButton_3_clicked();

  void on_comboxBox_portName_currentIndexChanged(const QString &arg1);

  void on_btn_start_serial_clicked(bool checked);

  void handleReadyRead();

  void on_btn_stop_serial_clicked();



  void on_actionStart_Record_triggered();

  void on_actionStop_Record_triggered();

  void updateLeft(QImage img);

  void updateRight(QImage img);

  void updateDepth(QImage img);

  bool drawImuInfo(double acc_x, double acc_y, double acc_z, double gyr_x,
                   double gyr_y, double gyr_z, double temp);

  void showUWB(QString str);

  void on_btn_start_imu_clicked();

private:
  Ui::MainWindow *ui;
  QSerialPort serial_port_;
  QSerialPortInfo serial_info_;


  QTimer *serial_timer_ = nullptr;

  QLabel *left_label_;
  QLabel *right_label_;
  QLabel *depth_label_;
  QLabel *info_label_;

  QTimer *cam_timer_;

  MYNTReader *mynt_reader_ = nullptr;

  SerialBuffer* serial_reader_ = nullptr;

  /// FILE SAVE
  QString save_dir = "";
  bool saving_flag = false;
  DatasetWriter *writer_ptr_;

  bool setupDrawImage();
};

#endif // MAINWINDOW_H
