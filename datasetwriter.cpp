#include "datasetwriter.h"

DatasetWriter::DatasetWriter(std::string dir_name) {
  father_dir = dir_name;
  if (father_dir.at(father_dir.size() - 1) != '/') {
    father_dir = dir_name + '/';
  }
  left_img_dir = dir_name + "left/";
  right_img_dir = dir_name + "right/";

  try {
    left_img_info_file.open(left_img_dir + "image.csv",
                            std::ios_base::out | std::ios::trunc);

  } catch (...) {
  }
}
