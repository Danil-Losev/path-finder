#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "structs.h"
#include <QImage>


namespace engine
{
namespace filters
{
class ImageProcessor
{
  public:
    ImageProcessor() = delete;
    ~ImageProcessor() = delete;
    static QImage applyFilter(const QImage &image, const FilterKernel &kernel);
};
} // namespace filters
} // namespace engine

#endif // IMAGEPROCESSOR_H
