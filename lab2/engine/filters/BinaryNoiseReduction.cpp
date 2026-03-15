#include "BinaryNoiseReduction.h"

static inline uchar toBinary(uchar v)
{
    return (v > 127) ? 255 : 0;
}

QImage engine::filters::BinaryNoiseReduction::erosion(const QImage &inputImage, int kernelSize)
{
    QImage sourceImage = inputImage;
    if (sourceImage.format() != QImage::Format_Grayscale8)
        sourceImage = sourceImage.convertToFormat(QImage::Format_Grayscale8);
    QImage resultImage(sourceImage.size(), QImage::Format_Grayscale8);

    int imageWidth = sourceImage.width();
    int imageHeight = sourceImage.height();
    int kernelRadius = kernelSize / 2;

    std::vector<const uchar *> sourceLines(imageHeight);
    for (int rowIndex = 0; rowIndex < imageHeight; ++rowIndex)
        sourceLines[rowIndex] = sourceImage.constScanLine(rowIndex);

    std::vector<uchar *> resultLines(imageHeight);
    for (int rowIndex = 0; rowIndex < imageHeight; ++rowIndex)
        resultLines[rowIndex] = resultImage.scanLine(rowIndex);

    for (int y = 0; y < imageHeight; ++y)
    {
        uchar *resultRow = resultLines[y];
        for (int x = 0; x < imageWidth; ++x)
        {
            bool isAllNeighborhoodWhite = true;
            for (int kernelOffsetY = -kernelRadius; kernelOffsetY <= kernelRadius && isAllNeighborhoodWhite;
                 ++kernelOffsetY)
            {
                int neighborY = std::clamp(y + kernelOffsetY, 0, imageHeight - 1);
                const uchar *sourceRow = sourceLines[neighborY];
                for (int kernelOffsetX = -kernelRadius; kernelOffsetX <= kernelRadius && isAllNeighborhoodWhite;
                     ++kernelOffsetX)
                {
                    int neighborX = std::clamp(x + kernelOffsetX, 0, imageWidth - 1);
                    if (sourceRow[neighborX] == 0)
                        isAllNeighborhoodWhite = false;
                }
            }
            resultRow[x] = isAllNeighborhoodWhite ? 255 : 0;
        }
    }
    return resultImage;
}

QImage engine::filters::BinaryNoiseReduction::dilation(const QImage &inputImage, int kernelSize)
{
    QImage sourceImage = inputImage;
    if (sourceImage.format() != QImage::Format_Grayscale8)
        sourceImage = sourceImage.convertToFormat(QImage::Format_Grayscale8);
    QImage resultImage(sourceImage.size(), QImage::Format_Grayscale8);

    int imageWidth = sourceImage.width();
    int imageHeight = sourceImage.height();
    int kernelRadius = kernelSize / 2;

    std::vector<const uchar *> sourceLines(imageHeight);
    for (int rowIndex = 0; rowIndex < imageHeight; ++rowIndex)
        sourceLines[rowIndex] = sourceImage.constScanLine(rowIndex);

    std::vector<uchar *> resultLines(imageHeight);
    for (int rowIndex = 0; rowIndex < imageHeight; ++rowIndex)
        resultLines[rowIndex] = resultImage.scanLine(rowIndex);

    for (int y = 0; y < imageHeight; ++y)
    {
        uchar *resultRow = resultLines[y];
        for (int x = 0; x < imageWidth; ++x)
        {
            bool isAnyNeighborhoodWhite = false;
            for (int kernelOffsetY = -kernelRadius; kernelOffsetY <= kernelRadius && !isAnyNeighborhoodWhite;
                 ++kernelOffsetY)
            {
                int neighborY = std::clamp(y + kernelOffsetY, 0, imageHeight - 1);
                const uchar *sourceRow = sourceLines[neighborY];
                for (int kernelOffsetX = -kernelRadius; kernelOffsetX <= kernelRadius && !isAnyNeighborhoodWhite;
                     ++kernelOffsetX)
                {
                    int neighborX = std::clamp(x + kernelOffsetX, 0, imageWidth - 1);
                    if (sourceRow[neighborX] == 255)
                        isAnyNeighborhoodWhite = true;
                }
            }
            resultRow[x] = isAnyNeighborhoodWhite ? 255 : 0;
        }
    }
    return resultImage;
}

QImage engine::filters::BinaryNoiseReduction::opening(const QImage &image, int kernelSize)
{
    return dilation(erosion(image, kernelSize), kernelSize);
}

QImage engine::filters::BinaryNoiseReduction::closing(const QImage &image, int kernelSize)
{
    return erosion(dilation(image, kernelSize), kernelSize);
}