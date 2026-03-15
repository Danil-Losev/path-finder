#ifndef BINARYGENERATOR_H
#define BINARYGENERATOR_H

#include <qimage.h>

namespace engine
{
namespace generators
{
class BinaryGenerator
{
  public:
    BinaryGenerator() = delete;
    ~BinaryGenerator() = delete;

    static QImage generate(const QImage &image, int threshold = 50);
    static QRgb findBackgroundColor(const QImage &image);
    static bool isBackgroundColor(const QRgb &color, const QRgb &backgroundColor, int32_t threshold = 16);
    static int triangleThreshold(const QImage &image);
    static int symmetricPeakThreshold(const QImage &image);
};
} // namespace generators
} // namespace engine

#endif // BINARYGENERATOR_H
