#include "datasetwriter.h"

DatasetWriter::DatasetWriter(std::string dir_name) {
  father_dir = dir_name;
  //  if (father_dir.at(father_dir.size() - 1) != '/') {
  //    father_dir = dir_name + '/';
  //  }
  left_img_dir = dir_name + "left/";
  right_img_dir = dir_name + "right/";

  left_img_info_file.open(left_img_dir + "image.csv",
                          std::ios_base::out | std::ios::trunc);
  right_img_info_file.open(right_img_dir + "image.csv",
                           std::ios_base::out | std::ios::trunc);

  imu_file.open(father_dir + "motion.csv",
                std::ios_base::out | std::ios::trunc);

  if (left_img_info_file.is_open() && right_img_info_file.is_open() &&
      imu_file.is_open()) {
    status = true;
  } else {

    std::cout << "some file not opened"
              << "\nleft image file:"
              << (left_img_info_file.is_open() ? "true" : "false")
              << " dir is:" << left_img_dir << "\nright imagefile:"
              << (right_img_info_file.is_open() ? "true" : "false")
              << " dir is:" << right_img_dir
              << "\n imu       file:" << (imu_file.is_open() ? "true" : "false")
              << "dir is :" << father_dir << std::endl;
  }
}


