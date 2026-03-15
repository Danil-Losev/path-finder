#ifndef CONTRASTSTRETCH_H
#define CONTRASTSTRETCH_H

#include <QImage>

namespace engine
{
namespace filters
{
class ContrastStretch
{
  public:
    ContrastStretch() = delete;
    ~ContrastStretch() = delete;
    static QImage apply(const QImage &image);
};
} // namespace filters
} // namespace engine

#endif // CONTRASTSTRETCH_H
