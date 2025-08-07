#ifndef WORK_H
#define WORK_H

#include <QObject>

class Work : public QObject
{
    Q_OBJECT

public:
    explicit Work(int index,QObject *parent = nullptr);

    void fractalTrigonometricSeries();
    void exponentialSum3D();
    void modularChaoticMap();
    void CircleArea();


signals:
    void progress(int index,int value);
    void finished(int colindex);

public slots:
    void Process();

private:
    int Roll();
    int Colindex;
};

#endif // WORK_H
