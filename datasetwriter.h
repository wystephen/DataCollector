#ifndef DATASETWRITER_H
#define DATASETWRITER_H

#include <fstream>
#include <iostream>

#include <deque>
#include <vector>

struct ImuData {
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

  std::string father_dir;

  std::string left_img_dir;
  std::string right_img_dir;

  std::ofstream left_img_info_file;
  std::ofstream right_img_info_file;

  std::ofstream imu_file;

  std::deque<ImuData> imu_buf_vec;
  std::deque<ImageInfo> img_info_vec;
};

#endif // DATASETWRITER_H
