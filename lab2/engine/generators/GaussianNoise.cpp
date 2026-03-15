#include "GaussianNoise.h"
#include <random>

QImage engine::generators::GaussianNoise::apply(const QImage &image, double mean, double stddev)
{
    QImage noisy = image.convertToFormat(QImage::Format_RGB32);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(mean, stddev);

    int w = noisy.width();
    int h = noisy.height();

    for (int y = 0; y < h; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(noisy.scanLine(y));
        for (int x = 0; x < w; ++x)
        {
            QColor color = QColor::fromRgb(line[x]);
            int r = std::clamp(int(color.red() + dist(gen)), 0, 255);
            int g = std::clamp(int(color.green() + dist(gen)), 0, 255);
            int b = std::clamp(int(color.blue() + dist(gen)), 0, 255);
            line[x] = qRgb(r, g, b);
        }
    }
    return noisy;
}
