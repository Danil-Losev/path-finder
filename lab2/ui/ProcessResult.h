#ifndef PROCESSRESULT_H
#define PROCESSRESULT_H

#include <QImage>
#include <QString>

struct ProcessResult
{
    QImage pathOnProcessed;
    QImage pathOnColorMap;
    QImage pathOnOriginal;
    QString description;
};

struct StepState
{
    QImage image;
    QString description;
};

#endif // PROCESSRESULT_H
