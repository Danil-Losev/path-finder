#include "GreyWorldFilter.h"

QImage engine::filters::GreyWorldFilter::apply(const QImage &image)
{
    QImage source = image;
    if (source.format() != QImage::Format_RGB32)
    {
        source = source.convertToFormat(QImage::Format_RGB32);
    }

    int32_t imageHeight = source.height();
    int32_t imageWidth = source.width();

    uint64_t totalPixels = static_cast<uint64_t>(imageWidth) * static_cast<uint64_t>(imageHeight);
    if (totalPixels >= UINT64_MAX)
    {
        return source;
    }

    uint64_t rSum = 0, gSum = 0, bSum = 0;
    for (int32_t y = 0; y < imageHeight; ++y)
    {
        const QRgb *line = reinterpret_cast<const QRgb *>(source.constScanLine(y));
        for (int32_t x = 0; x < imageWidth; ++x)
        {
            rSum += qRed(line[x]);
            gSum += qGreen(line[x]);
            bSum += qBlue(line[x]);
        }
    }

    long double rAvg = static_cast<long double>(rSum) / totalPixels;
    long double gAvg = static_cast<long double>(gSum) / totalPixels;
    long double bAvg = static_cast<long double>(bSum) / totalPixels;
    long double grayAvg = (rAvg + gAvg + bAvg) / 3.0L;

    const long double redCoeff = (rAvg == 0.0L) ? 1.0L : grayAvg / rAvg;
    const long double greenCoeff = (gAvg == 0.0L) ? 1.0L : grayAvg / gAvg;
    const long double blueCoeff = (bAvg == 0.0L) ? 1.0L : grayAvg / bAvg;

    QImage result(source.size(), QImage::Format_RGB32);
    for (int32_t y = 0; y < imageHeight; ++y)
    {
        const QRgb *sourceLine = reinterpret_cast<const QRgb *>(source.constScanLine(y));
        QRgb *resultLine = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int32_t x = 0; x < imageWidth; ++x)
        {
            int32_t red = std::clamp(static_cast<int32_t>(qRed(sourceLine[x]) * redCoeff), 0, 255);
            int32_t green = std::clamp(static_cast<int32_t>(qGreen(sourceLine[x]) * greenCoeff), 0, 255);
            int32_t blue = std::clamp(static_cast<int32_t>(qBlue(sourceLine[x]) * blueCoeff), 0, 255);
            resultLine[x] = qRgb(red, green, blue);
        }
    }
    return result;
}