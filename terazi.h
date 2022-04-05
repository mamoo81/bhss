#ifndef TERAZI_H
#define TERAZI_H

#include <QtCore>
#include <QThread>
#include <QObject>
#include <QtSerialPort>

class Terazi : public QThread
{
    Q_OBJECT

    void run();

public:
    Terazi();

    bool stop = false;

signals:
    void StabilAgirlikOkununca(double);
    void AgirlikOkununca(double);
};

#endif // TERAZI_H
