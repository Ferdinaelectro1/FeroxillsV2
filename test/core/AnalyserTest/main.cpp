#include <QCoreApplication>
#include <QDebug>
#include "../../../src/core/analyser/SamplesAnalyser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Starting analyser test";
    const QVector<double> _winAnalyse = {-3,-2,-1,-0.9, - 0.1, 0.2, 0.4 , 3, 6,-5,-3,-2,-0.1,0.2,0.6,1,3,5};
    SamplesAnalyser analyser;
    SamplesParameter parameter =  analyser.getSamplesParameter(_winAnalyse);
    qDebug() << "Result";
    qDebug() << "Max Voltage  = "<< parameter.voltage_max;
    qDebug() << "Min Voltage  = "<< parameter.voltage_min;
    qDebug() << "First Rising  = "<< parameter._first_rising_pos;
    qDebug() << "Second Rising  = "<< parameter._second_rising_pos;
    qDebug() << "Samples Period  = "<< parameter.periodSamples;

    return 0;
}
