#include "BinaryGenerator.h"

#include <qmap.h>

int engine::generators::BinaryGenerator::symmetricPeakThreshold(const QImage &image)
{
    std::vector<int> hist(256, 0);
    int width = image.width(), height = image.height();
    QImage gray = image.convertToFormat(QImage::Format_Grayscale8);

    for (int y = 0; y < height; ++y)
    {
        const uchar *line = gray.constScanLine(y);
        for (int x = 0; x < width; ++x)
            ++hist[line[x]];
    }
    int peak1 = 0, peak2 = 0;
    int peak1Val = 0, peak2Val = 0;

    for (int i = 0; i < 256; ++i)
    {
        if (hist[i] > peak1Val)
        {
            peak1 = i;
            peak1Val = hist[i];
        }
    }

    for (int i = 0; i < 256; ++i)
    {
        if (std::abs(i - peak1) > 100 && hist[i] > peak2Val)
        {
            peak2 = i;
            peak2Val = hist[i];
        }
    }

    int tLow = std::min(peak1, peak2), tHigh = std::max(peak1, peak2);
    int minVal = hist[tLow], minIdx = tLow;
    for (int i = tLow + 1; i < tHigh; ++i)
    {
        if (hist[i] < minVal)
        {
            minVal = hist[i];
            minIdx = i;
        }
    }

    int threshold = (minIdx != tLow) ? minIdx : ((peak1 + peak2) / 2);
    return threshold;
}

int engine::generators::BinaryGenerator::triangleThreshold(const QImage &image)
{
    std::vector<int> hist(256, 0);
    int width = image.width(), height = image.height();

    for (int y = 0; y < height; ++y)
    {
        const QRgb *line = reinterpret_cast<const QRgb *>(image.constScanLine(y));
        for (int x = 0; x < width; ++x)
        {
            int gray = qGray(line[x]);
            ++hist[gray];
        }
    }

    int maxIdx = 0, maxVal = hist[0];
    for (int i = 1; i < 256; ++i)
        if (hist[i] > maxVal)
        {
            maxVal = hist[i];
            maxIdx = i;
        }

    int left = 0, right = 255;
    bool searchRight = (maxIdx < 128);

    int edgeIdx = searchRight ? right : left;

    double bestDist = -1;
    int bestIdx = maxIdx;
    for (int i = std::min(maxIdx, edgeIdx); i <= std::max(maxIdx, edgeIdx); ++i)
    {
        double dx = maxIdx - edgeIdx;
        double dy = hist[maxIdx] - hist[edgeIdx];
        double dist = std::abs(dy * (i - edgeIdx) - dx * (hist[i] - hist[edgeIdx])) / std::sqrt(dx * dx + dy * dy);
        if (dist > bestDist)
        {
            bestDist = dist;
            bestIdx = i;
        }
    }
    return bestIdx;
}

// QImage engine::generators::BinaryGenerator::generate(const QImage& image)
//{
//     //int threshold = triangleThreshold(image);
//	int threshold = symmetricPeakThreshold(image);
//     QImage binaryImage(image.size(), QImage::Format_Grayscale8);
//     int width = image.width(), height = image.height();
//
//     for (int y = 0; y < height; ++y) {
//         const QRgb* sourceLine = reinterpret_cast<const QRgb*>(image.constScanLine(y));
//         uchar* binaryLine = binaryImage.scanLine(y);
//         for (int x = 0; x < width; ++x) {
//             int gray = qGray(sourceLine[x]);
//             binaryLine[x] = (gray > threshold) ? 255 : 0;
//         }
//     }
//     return binaryImage;
// }

QImage engine::generators::BinaryGenerator::generate(const QImage &image, int threshold)
{
    QRgb backgroundColor = findBackgroundColor(image);
    QImage binaryImage(image.size(), QImage::Format_Grayscale8);
    int width = image.width(), height = image.height();
    for (int y = 0; y < height; ++y)
    {
        const QRgb *sourceLine = reinterpret_cast<const QRgb *>(image.constScanLine(y));
        uchar *binaryLine = binaryImage.scanLine(y);
        for (int x = 0; x < width; ++x)
        {
            binaryLine[x] = isBackgroundColor(sourceLine[x], backgroundColor, threshold) ? 0 : 255;
        }
    }
    return binaryImage;
}

QRgb engine::generators::BinaryGenerator::findBackgroundColor(const QImage &image)
{
    QMap<QRgb, int32_t> colorCount;
    int32_t width = image.width();
    int32_t height = image.height();

    for (int32_t x = 0; x < width; ++x)
    {
        for (int32_t y = 0; y < height; ++y)
        {
            QRgb color = image.pixel(x, y);
            colorCount[color]++;
        }
    }

    QRgb backgroundColor = 0;
    int32_t maxCount = 0;
    for (auto it = colorCount.begin(); it != colorCount.end(); ++it)
    {
        if (it.value() > maxCount)
        {
            maxCount = it.value();
            backgroundColor = it.key();
        }
    }
    return backgroundColor;
}

bool engine::generators::BinaryGenerator::isBackgroundColor(const QRgb &color, const QRgb &backgroundColor,
                                                            int32_t threshold)
{
    int32_t dr = std::abs(qRed(color) - qRed(backgroundColor));
    int32_t dg = std::abs(qGreen(color) - qGreen(backgroundColor));
    int32_t db = std::abs(qBlue(color) - qBlue(backgroundColor));
    return dr <= threshold && dg <= threshold && db <= threshold;
}