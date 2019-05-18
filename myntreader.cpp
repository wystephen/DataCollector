#include "myntreader.h"

MYNTEYE_USE_NAMESPACE





MYNTReader::MYNTReader(){
    DeviceInfo dev_info;
    bool cam_ok = selectDevice(&dev_info);
    if(cam_ok){
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

    }
}


bool MYNTReader::selectDevice(DeviceInfo *dev_info) {
    std::vector<DeviceInfo> dev_infos = mynt_cam_.GetDeviceInfos();
    std::size_t n = dev_infos.size();
    if (n <= 0) {
        QMessageBox::warning(nullptr,
                             tr("ERROR"),
                             tr("No MYNT@ EYE Depth Camera detected"),
                             QMessageBox::Ok);

        return false;
    } else {
        if (n == 1) {
            *dev_info = dev_infos[0];
            return true;
        } else { // > 1
//            QStringList items;
//            for (auto &&info : dev_infos) {
//                items << QString("%1 | %2").arg(info.index).arg(info.name.c_str());
//            }

//            bool ok;
//            QString item = QInputDialog::getItem(
//                this, tr("Select"), tr("MYNT® EYE cameras"), items, 0, false, &ok);
//            if (ok) {
//                QStringList parts = item.split("|");
//                if (parts.length() > 0) {
//                    QString index = parts[0].trimmed();
//                    for (auto &&info : dev_infos) {
//                        if (QString::number(info.index) == index) {
//                            *dev_info = info;
//                            return true;
//                        }
//                    }
//                }
//            }
        }
    }
    return false;
}


void MYNTReader::run(){
    while(true){
        std::cout << "started run" << std::endl;
    if(mynt_cam_.HasStreamDataEnabled()){
        auto &&left = mynt_cam_.GetStreamData(ImageType::IMAGE_LEFT_COLOR);
        auto &&right = mynt_cam_.GetStreamData(ImageType::IMAGE_RIGHT_COLOR);
        auto &&depth = mynt_cam_.GetStreamData(ImageType::IMAGE_DEPTH);

        auto write_image = [](QImage img,QString path_str){
            img.save(path_str);
        };

        if(left.img){
            auto &&img = left.img->To(ImageFormat::COLOR_RGB);
            QImage image(img->data(),
                         img->width(),
                         img->height(),
                         QImage::Format_RGB888);

            left_index+=1;
            if(left_index%3==0){
                QImage small_img = image.scaledToWidth(display_width_);
//                emit newLeft(&small_img);
                left_index = 0;
            }

        }
    }else{
        msleep(10);
    }
   }



}
