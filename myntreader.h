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

private:
    Camera mynt_cam_;

    DatasetWriter *writher_ptr_;

    bool selectDevice(DeviceInfo *dev_info);

signals:

public slots:
};

#endif // MYNTREADER_H
