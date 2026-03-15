#ifndef BINARYNOISEREDUCTION_H
#define BINARYNOISEREDUCTION_H

#include "structs.h"
#include <QImage>

namespace engine
{
namespace filters
{
class BinaryNoiseReduction
{
  public:
    BinaryNoiseReduction() = delete;
    ~BinaryNoiseReduction() = delete;

    static QImage erosion(const QImage &image, int kernelSize);
    static QImage dilation(const QImage &image, int kernelSize);
    static QImage opening(const QImage &image, int kernelSize);
    static QImage closing(const QImage &image, int kernelSize);
};
} // namespace filters
} // namespace engine

#endif // BINARYNOISEREDUCTION_H
