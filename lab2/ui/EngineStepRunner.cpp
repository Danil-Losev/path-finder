#include "EngineStepRunner.h"
#include "../engine/binary/BinaryImageInfo.h"
#include "../engine/filters/BinaryNoiseReduction.h"
#include "../engine/filters/ContrastStretch.h"
#include "../engine/filters/GreyWorldFilter.h"
#include "../engine/filters/ImageProcessor.h"
#include "../engine/filters/KernelCreator.h"
#include "../engine/filters/MedianFilter.h"
#include "../engine/generators/BinaryGenerator.h"
#include "../engine/generators/GaussianNoise.h"
#include "../engine/painter/ImagePainter.h"
#include "../engine/searcher/ArrowSearcher.h"
#include "../engine/searcher/PathFinder.h"
#include <QImage>
#include <stdexcept>


EngineStepRunner::EngineStepRunner(const ImageInfo &imgInfo)
{
    generateAllSteps(imgInfo);
}

int EngineStepRunner::count() const
{
    return int(steps.size());
}
StepState EngineStepRunner::step(int idx) const
{
    return steps.at(idx);
}
ProcessResult EngineStepRunner::finalResult() const
{
    return result;
}

void EngineStepRunner::generateAllSteps(const ImageInfo &imgInfo)
{
    QImage originalImage;
    if (!originalImage.load(imgInfo.pathToImage.c_str()))
    {
        throw std::runtime_error("Image load error!");
    }
    steps.push_back({originalImage, "Исходное изображение"});

    QImage processedImg = originalImage;
    if (imgInfo.filterConfig.medianKernelSize > 0)
    {
        processedImg = engine::filters::MedianFilter::apply(processedImg, imgInfo.filterConfig.medianKernelSize);
        steps.push_back({processedImg, "Медианный фильтр"});
    }
    if (imgInfo.greyWorld)
    {
        processedImg = engine::filters::GreyWorldFilter::apply(processedImg);
        steps.push_back({processedImg, "Баланс белого (Grey World)"});
    }
    if (imgInfo.contrastStretch)
    {
        processedImg = engine::filters::ContrastStretch::apply(processedImg);
        steps.push_back({processedImg, "Растяжение контраста"});
    }
    if (imgInfo.filterConfig.gaussianKernelSize > 0)
    {
        processedImg = engine::filters::ImageProcessor::applyFilter(
            processedImg, engine::filters::KernelCreator::createGaussianBlurKernel(
                              imgInfo.filterConfig.gaussianKernelSize, imgInfo.filterConfig.gaussianKernelSigma));
        steps.push_back({processedImg, "Гауссово размытие"});
    }
    QImage imgBin = engine::generators::BinaryGenerator::generate(processedImg, imgInfo.binaryThreshold);
    steps.push_back({imgBin, "Бинаризация"});
    if (imgInfo.filterConfig.openingKernelSize > 0)
    {
        imgBin = engine::filters::BinaryNoiseReduction::opening(imgBin, imgInfo.filterConfig.openingKernelSize);
        steps.push_back({imgBin, "Морфологическое раскрытие (opening)"});
    }
    if (imgInfo.filterConfig.closingKernelSize > 0)
    {
        imgBin = engine::filters::BinaryNoiseReduction::closing(imgBin, imgInfo.filterConfig.closingKernelSize);
        steps.push_back({imgBin, "Морфологическое замыкание (closing)"});
    }

    engine::binary::BinaryImageInfo info(imgBin, imgInfo.groupingThreshold, imgInfo.noiseMaxSize);
    auto labels = info.getBinaryLabels();
    auto props = info.getRegionProperties();
    auto groups = info.getGroupedLabels();

    for (const auto group : groups)
    {
        QString groupInfo = "Группа " + QString::number(group.first + 1) +
                            " кол-во объектов: " + QString::number(group.second.size()) + "\n"; 
        QImage emptyImg(imgBin.size(), QImage::Format_RGB32);
        emptyImg = engine::painter::ImagePainter::recolorLabels(emptyImg, labels, group.second, qRgb(255, 0, 0));

        steps.push_back({emptyImg, groupInfo});
    }


    QImage colorMap = engine::painter::ImagePainter::colorLabelsByGroups(groups, labels);
    steps.push_back({colorMap, "Выделение всех целевых компонент"});

    int redLabel = engine::searcher::ArrowSearcher::findRedArrow(originalImage, labels);
    engine::binary::LabelGroup arrowGroup = engine::searcher::ArrowSearcher::findArrowGroup(groups, redLabel);

    if (!arrowGroup.empty())
    {
        colorMap = engine::painter::ImagePainter::recolorLabels(colorMap, labels, arrowGroup, qRgb(0, 255, 255));
        steps.push_back({colorMap, "Выделение группы стрелок"});
    }
    if (redLabel > 0)
    {
        colorMap = engine::painter::ImagePainter::recolorLabel(colorMap, labels, redLabel, qRgb(255, 255, 0));
        steps.push_back({colorMap, "Выделение красной стрелки"});
    }
    auto [treasureLabel, path] =
        engine::searcher::PathFinder::findArrowPathAndTreasure(props, labels, redLabel, arrowGroup);
    QImage pathOnColor = engine::painter::ImagePainter::drawPathByLabels(colorMap, path, props, Qt::red, 6);
    steps.push_back({pathOnColor, "Путь по стрелкам"});

    processedImg = engine::painter::ImagePainter::drawPathByLabels(processedImg, path, props, Qt::red, 6);
    originalImage = engine::painter::ImagePainter::drawPathByLabels(originalImage, path, props, Qt::red, 6);

    if (treasureLabel > 0)
    {
        pathOnColor =
            engine::painter::ImagePainter::drawBoundingBoxForLabel(pathOnColor, labels, treasureLabel, Qt::magenta, 4);
        processedImg =
            engine::painter::ImagePainter::drawBoundingBoxForLabel(processedImg, labels, treasureLabel, Qt::magenta, 4);
        originalImage = engine::painter::ImagePainter::drawBoundingBoxForLabel(originalImage, labels, treasureLabel,
                                                                               Qt::magenta, 4);
        steps.push_back({pathOnColor, "Клад выделен рамкой"});
    }

    result.pathOnProcessed = processedImg;
    result.pathOnColorMap = pathOnColor;
    result.pathOnOriginal = originalImage;
    result.description = "Обработка завершена";
}