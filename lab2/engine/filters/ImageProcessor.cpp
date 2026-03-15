#include "ImageProcessor.h"

QImage engine::filters::ImageProcessor::applyFilter(const QImage &image, const engine::filters::FilterKernel &kernel)
{
    QImage source = image;
    if (source.format() != QImage::Format_RGB32)
    {
        source = source.convertToFormat(QImage::Format_RGB32);
    }

    int32_t imageWidth = source.width();
    int32_t imageHeight = source.height();
    int32_t kernelWidth = kernel.kernel[0].size();
    int32_t kernelHeight = kernel.kernel.size();
    int32_t kernelCenterX = kernelWidth / 2;
    int32_t kernelCenterY = kernelHeight / 2;

    QImage result(source.size(), QImage::Format_RGB32);

    std::vector<const QRgb *> sourceLines(imageHeight);
    for (int32_t row = 0; row < imageHeight; ++row)
        sourceLines[row] = reinterpret_cast<const QRgb *>(source.constScanLine(row));

    for (int32_t y = 0; y < imageHeight; ++y)
    {
        QRgb *resultLine = reinterpret_cast<QRgb *>(result.scanLine(y));

        std::vector<const QRgb *> neighborLines(kernelHeight);
        for (int32_t j = 0; j < kernelHeight; ++j)
        {
            int32_t neighborY = std::clamp(y + j - kernelCenterY, 0, imageHeight - 1);
            neighborLines[j] = sourceLines[neighborY];
        }

        for (int32_t x = 0; x < imageWidth; ++x)
        {
            double sumRed = 0.0, sumGreen = 0.0, sumBlue = 0.0;

            for (int32_t j = 0; j < kernelHeight; ++j)
            {
                const QRgb *sourceLine = neighborLines[j];
                for (int32_t i = 0; i < kernelWidth; ++i)
                {
                    int32_t sourceX = std::clamp(x + i - kernelCenterX, 0, imageWidth - 1);
                    QRgb sourcePixel = sourceLine[sourceX];
                    double kernelValue = kernel.kernel[j][i];
                    sumRed += qRed(sourcePixel) * kernelValue;
                    sumGreen += qGreen(sourcePixel) * kernelValue;
                    sumBlue += qBlue(sourcePixel) * kernelValue;
                }
            }

            int32_t resultRed = std::clamp(static_cast<int32_t>(std::round(sumRed)), 0, 255);
            int32_t resultGreen = std::clamp(static_cast<int32_t>(std::round(sumGreen)), 0, 255);
            int32_t resultBlue = std::clamp(static_cast<int32_t>(std::round(sumBlue)), 0, 255);

            resultLine[x] = qRgb(resultRed, resultGreen, resultBlue);
        }
    }
    return result;
}