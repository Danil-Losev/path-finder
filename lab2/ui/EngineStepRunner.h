#ifndef ENGINESTEPRUNNER_H
#define ENGINESTEPRUNNER_H

#include "ImageInfo.h"
#include "ProcessResult.h"
#include <QString>
#include <vector>

class EngineStepRunner
{
  public:
    EngineStepRunner(const ImageInfo &imgInfo);

    int count() const;
    StepState step(int idx) const;
    ProcessResult finalResult() const;

  private:
    std::vector<StepState> steps;
    ProcessResult result;
    void generateAllSteps(const ImageInfo &imgInfo);
};

#endif // ENGINESTEPRUNNER_H
