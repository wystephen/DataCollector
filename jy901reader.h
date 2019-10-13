#ifndef JY901READER_H
#define JY901READER_H

#include <QObject>

class JY901Reader : public QObject
{
    Q_OBJECT
public:
    explicit JY901Reader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // JY901READER_H
