#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <QObject>

namespace engine
{
namespace filters
{
class MedianFilter
{
  public:
    MedianFilter() = delete;
    ~MedianFilter() = delete;
    static QImage apply(const QImage &image, int kernelSize);
};
} // namespace filters
} // namespace engine

#endif // MEDIANFILTER_H
