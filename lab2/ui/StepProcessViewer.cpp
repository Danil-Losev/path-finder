#include "StepProcessViewer.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

StepProcessViewer::StepProcessViewer(EngineStepRunner *runner, QWidget *parent) : QDialog(parent), runner(runner)
{
    setWindowTitle("Пошаговый режим");
    QVBoxLayout *layout = new QVBoxLayout(this);
    imageLabel = new QLabel();
    stepText = new QLabel();
    nextButton = new QPushButton("Следующий шаг");

    layout->addWidget(imageLabel);
    layout->addWidget(stepText);
    layout->addWidget(nextButton);

    connect(nextButton, &QPushButton::clicked, this, &StepProcessViewer::onNextStep);
    updateStep();
}

void StepProcessViewer::updateStep()
{
    if (currentStep < runner->count())
    {
        StepState step = runner->step(currentStep);
        imageLabel->setPixmap(QPixmap::fromImage(step.image).scaled(450, 450, Qt::KeepAspectRatio));
        stepText->setText(step.description);
    }
    else
    {
        ProcessResult res = runner->finalResult();
        imageLabel->setPixmap(QPixmap::fromImage(res.pathOnOriginal).scaled(450, 450, Qt::KeepAspectRatio));
        stepText->setText(res.description);
    }
}
void StepProcessViewer::onNextStep()
{
    if (currentStep >= runner->count())
    {
        this->close();
    }
    currentStep++;
    if (currentStep == runner->count())
    {
        nextButton->setText("Завершить");
    }
    updateStep();
}