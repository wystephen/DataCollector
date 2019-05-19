#ifndef DATASETWRITER_H
#define DATASETWRITER_H

#include <fstream>
#include <iostream>

#include <chrono>

#include <deque>
#include <vector>

#include <thread>
#include <functional>

struct ImuDataBlock {
  double acc[3];
  double gyr[3];
  double time_stamp;
  double sys_time_stamp;
};

/**
 * @brief get_time_now
 * @return time stampe of now in seconds.
 */
inline double get_time_now(){
    auto d = std::chrono::system_clock::now().time_since_epoch();
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(d).count())/1000.0;
}

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
      return true;
  }

  bool RecordAccData(double acc_x,
                double acc_y,
                double acc_z,
                     double time_stamp){
      std::cout.precision(20);
//      std::cout << "acc time:" << time_stamp << std::endl;
      imu_file << "acc," << time_stamp << "," << get_time_now() << ","
               << acc_x << "," << acc_y << "," << acc_z
               << ",0,0,0\n";
      return true;
  }
  bool RecordGyrData(double gyr_x,
                double gyr_y,
                double gyr_z,
                     double time_stamp){
      std::cout.precision(20);
//      std::cout << "gyr time:" <<  time_stamp << std::endl;
      imu_file <<"gyr," << time_stamp << "," << get_time_now() << ","
               << "0,0,0,"
               << gyr_x << "," << gyr_y << "," << gyr_z << "\n";
      return true;
  }

  std::string RecordLeftImage(double time_stamp,
                              double exporture_time,
                              int frame_id){
      char frame_id_char[100];
      sprintf(frame_id_char,"%010d",frame_id);
      std::string frame_id_str(frame_id_char);
      left_img_info_file << frame_id_str.c_str() << ","
                         << time_stamp << ","
                         << get_time_now() << ","
                         << exporture_time << "\n";

      return left_img_dir + frame_id_str+".jpg";

  }
  std::string RecordRightImage(double time_stamp,
                               double exporture_time,
                               int frame_id){

      char frame_id_char[100];
      sprintf(frame_id_char,"%010d",frame_id);
      std::string frame_id_str(frame_id_char);
      right_img_info_file<< frame_id_str.c_str() << ","
                         << time_stamp << ","
                         << get_time_now() << ","
                         << exporture_time << "\n";

      return right_img_dir+ frame_id_str+".jpg";

  }


  bool Flush(){
      left_img_info_file.flush();
      right_img_info_file.flush();
      imu_file.flush();
      return true;
  }

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
