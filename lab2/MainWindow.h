
#pragma once
#include "ui/EngineStepRunner.h"
#include "ui/GalleryView.h"
#include "ui/ImageInfo.h"
#include "ui/ResultViewer.h"
#include "ui/StepProcessViewer.h"
#include <QMainWindow>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    MainWindow();

  private slots:
    void onInstantResult(const ImageInfo &info);
    void onStepwiseResult(const ImageInfo &info);

  private:
    GalleryView *gallery;
    std::vector<ImageInfo> images;
};