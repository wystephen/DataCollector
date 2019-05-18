#ifndef MYNTREADER_H
#define MYNTREADER_H

#include <QObject>

#include <QThread>

class MYNTReader : public QThread
{
    Q_OBJECT
public:
    explicit MYNTReader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MYNTREADER_H
