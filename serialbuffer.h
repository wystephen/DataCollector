#ifndef SERIALBUFFER_H
#define SERIALBUFFER_H

#include <QObject>
#include <QThread>
#include <QMessageBox>



class SerialBuffer :public QThread
{
    Q_OBJECT
public:
    SerialBuffer();
};

#endif // SERIALBUFFER_H
