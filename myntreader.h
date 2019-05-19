#ifndef MYNTREADER_H
#define MYNTREADER_H

#include <QObject>

#include <QThread>

#include <QMessageBox>


#include <mynteyed/camera.h>
#include <mynteyed/utils.h>

#include <datasetwriter.h>

MYNTEYE_USE_NAMESPACE

/**
 * @brief The MYNTReader class
 * Simply read all stream data and motion information data.
 */
class MYNTReader : public QThread
{
    Q_OBJECT
public:
    explicit MYNTReader();

    void stopThread();

    void startWrite(std::string dir_str);

    void stopWrite();

    int getDisplayWidth(){
        return display_width_;
    }

    int setDisplayWidth(const int display_width){
        display_width_ = display_width;
        return display_width_;
    }


protected:
    void run() Q_DECL_OVERRIDE;

private:
    Camera mynt_cam_;

    DatasetWriter *writer_ptr_=nullptr;


    /**
     * @brief selectDevice
     * Select default device. without other option.
     * @param dev_info
     * @return
     */
    bool selectDevice(DeviceInfo *dev_info);

    QAtomicInt write_flag_=0;

    int left_index=0;
    int right_index = 0;
    int depth_index = 0;

    int display_width_ = 200;


signals:
    void newLeft(QImage img);

    void newRight(QImage img);

    void newDepth(QImage img);

    void newIMU(double acc_x,double acc_y,double acc_z,
                double gyr_x,double gyr_y, double gyr_z,
                double time);

public slots:
};

#endif // MYNTREADER_H
