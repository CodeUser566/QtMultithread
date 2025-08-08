#include "Work.h"
#include <qdebug.h>
#include <QtMath>
#include <random>


int Work::Roll()
{
    thread_local std::mt19937 gen(std::random_device{}());
    thread_local std::uniform_int_distribution<int> dist(1,100);
    return dist(gen);
}


void Work::fractalTrigonometricSeries()
{
    for(int p = 0; p <= 100; ++p) {
        double x = 0;
        for(int i = 0; i < 200; ++i) {
                double result = 0.0;
                for (int j = 1; j <= i; ++j) {
                    double term = 0.0;
                    for (int j = 1; j <= 100; ++j) {
                        term += qSin(qSqrt(i * j) * M_PI / 180.0);
                    }
                    result += qAbs(qTan(term) * qLn(i + 1));
                }
            }
                emit progress(Colindex,p);
        }

    }


void Work::exponentialSum3D()
{
    for(int p = 0; p <= 100; ++p) {
        for (int i = 0; i < 6; ++i) {
            double sum = 0.0;
            double sum2 = sum;
            const double step = 0.05;
            for (double x = -i; x <= i; x += step) {
                for (double y = -i; y <= i; y += step) {
                    for (double z = -i; z <= i; z += step) {
                        double r = qSqrt(x*x + y*y + z*z);
                        if (r > 0) {
                            sum += qExp(-qAbs(qSin(x) * qCos(y) * qTan(z)) / r);
                            sum2 += qExp(-qAbs(qSin(x) * qCos(y) * qTan(z)) / r);
                        }
                    }
                }
            }
        }
        emit progress(Colindex,p);
    }
}

void Work::modularChaoticMap()
{
    for(int p = 0; p <= 100; ++p) {
        double x = 0;
        for(int i = 0; i < 5; ++i) {
            const int mod = 10000;
            double x = 0.5;
            double y = 0.5;
            double total = 0.0;
            for (int j = 0; j < i; ++j) {
                double xn = qSin(std::fmod(y * M_PI * 7, M_PI));
                double yn = qCos(std::fmod(x * M_PI * 3, M_PI));
                x = std::fmod(qAbs(xn) + 0.0001, 1.0);
                y = std::fmod(qAbs(yn) + 0.0001, 1.0);

                double val = qSqrt(qPow(qSin(x * M_PI * 100), 2) +
                                   qPow(qCos(y * M_PI * 100), 2));
                total += qAtan2(val, 1.0 - val);
            }
        }
        emit progress(Colindex,p);
    }
}

void Work::CircleArea()
{
    for(int p = 0; p <= 100; ++p) {
        // «тяжёлый» подсчёт
        double x = 0;
        for(int i = 0; i < 20000; ++i) {
            x += qSin(i) * qCos(i) / (i+1);
        }
        emit progress(Colindex,p);
    }
}

Work::Work(int index, QObject *parent):
    Colindex(index),QObject(parent)
{}


void Work::Process(){
    int number = Roll();
    if(number >= 25)
    {
        fractalTrigonometricSeries();
    }
    else if(number >= 50)
    {
        exponentialSum3D();
    }
    else if(number >=75)
    {
        modularChaoticMap();
    }
    else if(number < 25 )
    {
        CircleArea();
    }
    else
    {
        qDebug() << "Ошибка в рандоме!";
    }
    emit finished(Colindex);
    }



