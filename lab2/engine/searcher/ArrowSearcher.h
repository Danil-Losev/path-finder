#ifndef ARROWSEARCHER_H
#define ARROWSEARCHER_H

#include "../binary/BinaryImageInfo.h"
#include <qimage.h>

namespace engine
{
namespace searcher
{
struct AreaColorStats
{
    int count = 0;
    long long sumR = 0, sumG = 0, sumB = 0;
};

class ArrowSearcher
{
  public:
    ArrowSearcher() = delete;
    ~ArrowSearcher() = delete;
    static int findRedArrow(const QImage &image, const engine::binary::BinaryLabels &labels);
    static engine::binary::LabelGroup findArrowGroup(const engine::binary::GroupedLabels &groupedLabels,
                                                     int targetLabel);
};
} // namespace searcher
} // namespace engine

#endif // ARROWSEARCHER_H
