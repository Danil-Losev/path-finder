#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

#include "../binary/BinaryImageInfo.h"
#include "../searcher/PathFinder.h"
#include <QImage>

namespace engine
{
namespace painter
{

using BinaryRegionProperties = std::vector<engine::binary::BinaryRegionProperties>;

class ImagePainter
{
  public:
    ImagePainter() = delete;
    ~ImagePainter() = delete;
    static QImage colorLabelsByGroups(const engine::binary::GroupedLabels &groups,
                                      const engine::binary::BinaryLabels &labels);
    static QImage recolorLabels(const QImage &image, const engine::binary::BinaryLabels &labels,
                                const engine::binary::LabelGroup &targetLabels, QRgb newColor = qRgb(255, 0, 0));
    static QImage recolorLabel(const QImage &image, const engine::binary::BinaryLabels &labels, int32_t targetLabel,
                               QRgb newColor = qRgb(255, 0, 0));
    static QImage drawLabelsCharacteristics(const QImage &image, const engine::binary::LabelesProperties &properties,
                                            int32_t axisLen = 50, QColor massColor = Qt::yellow,
                                            QColor bboxColor = Qt::red, QColor medianColor = Qt::green,
                                            QColor axisColor = Qt::black);
    static QImage drawBoundingBoxForLabel(const QImage &image, const engine::binary::BinaryLabels &labels,
                                          int32_t targetLabel, QColor boxColor = Qt::red, int32_t thickness = 2);
    static QImage drawPathByLabels(const QImage &image, const engine::searcher::PathToTreasure &path,
                                   const engine::binary::LabelesProperties &labelsProp, QColor pathColor = Qt::white,
                                   int32_t thickness = 6);
};
}; // namespace painter
}; // namespace engine

#endif // IMAGEPAINTER_H
