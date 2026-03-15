#ifndef STEPPROCESSVIEWER_H
#define STEPPROCESSVIEWER_H

#include "EngineStepRunner.h"
#include <QDialog>
#include <QLabel>

class StepProcessViewer : public QDialog
{
    Q_OBJECT
  public:
    StepProcessViewer(EngineStepRunner *runner, QWidget *parent = nullptr);

  private slots:
    void onNextStep();

  private:
    EngineStepRunner *runner;
    QLabel *imageLabel;
    QLabel *stepText;
    QPushButton *nextButton;
    int currentStep = 0;

    void updateStep();
};

#endif // STEPPROCESSVIEWER_H
