#ifndef DATASETWRITER_H
#define DATASETWRITER_H

#include <fstream>
#include <iostream>

#include <deque>
#include <vector>

struct ImuDataBlock {
  double acc[3];
  double gyr[3];
  double time_stamp;
  double sys_time_stamp;
};

struct ImageInfo {
  double left_time;
  double right_time;

  int left_id;
  int right_id;

  double sys_time_stamp;
};

class DatasetWriter {
public:
  DatasetWriter(std::string dir_name);

  bool Close(){
      left_img_info_file.close();
      right_img_info_file.close();
      imu_file.close();
  }

  bool RecordAccData(double acc_x,
                double acc_y,
                double acc_z,
                double time_stamp);
  bool RecordGyrData(double gyr_x,
                double gyr_y,
                double gyr_z,
                double time_stamp);

  std::string father_dir;

  std::string left_img_dir;
  std::string right_img_dir;

  std::ofstream left_img_info_file;
  std::ofstream right_img_info_file;

  std::ofstream imu_file;

  std::deque<ImuDataBlock> imu_buf_vec;
  std::deque<ImageInfo> img_info_vec;

  inline bool IsValid(){
      return status;
  }

  bool status = false;
};

#endif // DATASETWRITER_H
