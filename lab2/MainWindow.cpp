#include "MainWindow.h"
#include <QScrollArea>

MainWindow::MainWindow() : QMainWindow()
{
    images = {
        {":/resources/simple/1.jpg", CLASS_SIMPLE, {0, 0, 0, 3, 0}, 8.0, 0.0, 60},
        {":/resources/simple/2.jpg", CLASS_SIMPLE, {0, 0, 0, 3, 0}, 7.0, 0.0, 60},
        {":/resources/simple/3.jpg", CLASS_SIMPLE, {0, 3, 0.5, 3, 0}, 4.0, 0.0, 60},
        {":/resources/medium/1.jpg", CLASS_MEDIUM, {5, 0, 0, 3, 0}, 7.0, 40.0, 60},
        {":/resources/medium/2.jpg", CLASS_MEDIUM, {7, 3, 0.9, 3, 0}, 9.0, 0.0, 60},
        {":/resources/medium/3.jpg", CLASS_MEDIUM, {7, 3, 1.1, 3, 0}, 9.0, 70.0, 60},
        {":/resources/medium/4.jpg", CLASS_MEDIUM, {5, 3, 0.8, 3, 0}, 11.0, 0.0, 60},
        {":/resources/medium/5.jpg", CLASS_MEDIUM, {5, 3, 3.5, 3, 0}, 5.0, 0.0, 50},
    };

    GalleryView *gallery = new GalleryView(images);

    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setWidget(gallery);

    setCentralWidget(scroll);
    connect(gallery, &GalleryView::instantRequested, this, &MainWindow::onInstantResult);
    connect(gallery, &GalleryView::stepwiseRequested, this, &MainWindow::onStepwiseResult);
    setMinimumSize(900, 700);
}

void MainWindow::onInstantResult(const ImageInfo &info)
{
    EngineStepRunner *runner = new EngineStepRunner(info);
    ResultViewer *viewer = new ResultViewer(runner->finalResult(), this);
    viewer->exec();
    delete runner;
    delete viewer;
}
void MainWindow::onStepwiseResult(const ImageInfo &info)
{
    EngineStepRunner *runner = new EngineStepRunner(info);
    StepProcessViewer *stepViewer = new StepProcessViewer(runner, this);
    stepViewer->exec();
    ResultViewer *resultViewer = new ResultViewer(runner->finalResult(), this);
    resultViewer->exec();
    delete runner;
    delete stepViewer;
}