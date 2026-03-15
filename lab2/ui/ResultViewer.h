#ifndef RESULTVIEWER_H
#define RESULTVIEWER_H

#include "ProcessResult.h"
#include <QDialog>

class ResultViewer : public QDialog
{
    Q_OBJECT
  public:
    ResultViewer(const ProcessResult &result, QWidget *parent = nullptr);
};

#endif // RESULTVIEWER_H
