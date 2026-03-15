#ifndef GAUSSIANNOISE_H
#define GAUSSIANNOISE_H

#include <qimage.h>

namespace engine
{
namespace generators
{
class GaussianNoise
{
  public:
    GaussianNoise() = delete;
    ~GaussianNoise() = delete;

    static QImage apply(const QImage &image, double mean, double stddev);
};
} // namespace generators
} // namespace engine

#endif // GAUSSIANNOISE_H
