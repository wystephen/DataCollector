#ifndef MYNTREADER_H
#define MYNTREADER_H

#include <QObject>

#include <QThread>

/**
 * @brief The MYNTReader class
 * Simply read all stream data and motion information data.
 */
class MYNTReader : public QThread
{
    Q_OBJECT
public:
    explicit MYNTReader();

signals:

public slots:
};

#endif // MYNTREADER_H
