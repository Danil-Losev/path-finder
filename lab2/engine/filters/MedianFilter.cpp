#include "MedianFilter.h"
#include <QImage>
#include <algorithm>
#include <array>


QImage engine::filters::MedianFilter::apply(const QImage &image, int kernelSize)
{
    QImage source = image.convertToFormat(QImage::Format_RGB32);
    int width = source.width();
    int height = source.height();
    int half = kernelSize / 2;
    int kernelArea = kernelSize * kernelSize;

    QImage result(width, height, QImage::Format_RGB32);

    std::vector<const QRgb *> sourceLines(height);
    std::vector<QRgb *> resultLines(height);
    for (int y = 0; y < height; ++y)
    {
        sourceLines[y] = reinterpret_cast<const QRgb *>(source.constScanLine(y));
        resultLines[y] = reinterpret_cast<QRgb *>(result.scanLine(y));
    }

    std::vector<int> reds(kernelArea), greens(kernelArea), blues(kernelArea);

    for (int y = 0; y < height; ++y)
    {
        QRgb *resultLine = resultLines[y];
        for (int x = 0; x < width; ++x)
        {
            int idx = 0;
            for (int ky = -half; ky <= half; ++ky)
            {
                int ny = std::clamp(y + ky, 0, height - 1);
                const QRgb *sourceLine = sourceLines[ny];
                for (int kx = -half; kx <= half; ++kx)
                {
                    int nx = std::clamp(x + kx, 0, width - 1);
                    QRgb pixel = sourceLine[nx];
                    reds[idx] = qRed(pixel);
                    greens[idx] = qGreen(pixel);
                    blues[idx] = qBlue(pixel);
                    ++idx;
                }
            }
            std::sort(reds.begin(), reds.begin() + kernelArea);
            std::sort(greens.begin(), greens.begin() + kernelArea);
            std::sort(blues.begin(), blues.begin() + kernelArea);
            int medianR = reds[kernelArea / 2];
            int medianG = greens[kernelArea / 2];
            int medianB = blues[kernelArea / 2];
            resultLine[x] = qRgb(medianR, medianG, medianB);
        }
    }
    return result;
}