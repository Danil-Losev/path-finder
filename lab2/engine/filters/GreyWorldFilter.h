#ifndef GREYWORLDFILTER_H
#define GREYWORLDFILTER_H

#include <QImage>

namespace engine
{
namespace filters
{
class GreyWorldFilter
{
  public:
    GreyWorldFilter() = delete;
    ~GreyWorldFilter() = delete;

    static QImage apply(const QImage &image);
};
} // namespace filters
} // namespace engine

#endif // GREYWORLDFILTER_H
