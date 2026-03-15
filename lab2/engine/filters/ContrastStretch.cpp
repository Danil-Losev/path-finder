#include "ContrastStretch.h"
#include <QDebug>

QImage engine::filters::ContrastStretch::apply(const QImage &image)
{
    QImage source = image;
    if (source.format() != QImage::Format_RGB32)
    {
        source = source.convertToFormat(QImage::Format_RGB32);
    }

    int32_t imageHeight = source.height();
    int32_t imageWidth = source.width();

    int32_t rMin = INT32_MAX, gMin = INT32_MAX, bMin = INT32_MAX;
    int32_t rMax = 0, gMax = 0, bMax = 0;

    for (int32_t y = 0; y < imageHeight; ++y)
    {
        const QRgb *line = reinterpret_cast<const QRgb *>(source.constScanLine(y));
        for (int32_t x = 0; x < imageWidth; ++x)
        {
            int32_t r = qRed(line[x]);
            int32_t g = qGreen(line[x]);
            int32_t b = qBlue(line[x]);
            rMin = std::min(rMin, r);
            rMax = std::max(rMax, r);
            gMin = std::min(gMin, g);
            gMax = std::max(gMax, g);
            bMin = std::min(bMin, b);
            bMax = std::max(bMax, b);
        }
    }

    QImage result(source.size(), QImage::Format_RGB32);
    for (int32_t y = 0; y < imageHeight; ++y)
    {
        const QRgb *sourceLine = reinterpret_cast<const QRgb *>(source.constScanLine(y));
        QRgb *resultLine = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int32_t x = 0; x < imageWidth; ++x)
        {
            int32_t r = qRed(sourceLine[x]);
            int32_t g = qGreen(sourceLine[x]);
            int32_t b = qBlue(sourceLine[x]);
            int32_t red = (rMax == rMin) ? r : (r - rMin) * 255 / (rMax - rMin);
            int32_t green = (gMax == gMin) ? g : (g - gMin) * 255 / (gMax - gMin);
            int32_t blue = (bMax == bMin) ? b : (b - bMin) * 255 / (bMax - bMin);
            resultLine[x] = qRgb(std::clamp(red, 0, 255), std::clamp(green, 0, 255), std::clamp(blue, 0, 255));
        }
    }
    return result;
}
