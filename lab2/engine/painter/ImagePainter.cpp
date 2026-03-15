#include "ImagePainter.h"
#include <QPainter>
#include <QSet>
#include <Qmap>
#include <random>

QImage engine::painter::ImagePainter::colorLabelsByGroups(const engine::binary::GroupedLabels &groups,
                                                          const engine::binary::BinaryLabels &labels)
{
    int32_t height = labels.size();
    int32_t width = labels[0].size();
    QImage result(width, height, QImage::Format_RGB32);

    std::unordered_map<int32_t, int32_t> labelToGroup;
    for (const auto &[group, vec] : groups)
    {
        for (int32_t label : vec)
        {
            labelToGroup[label] = group;
        }
    }

    std::unordered_map<int32_t, QColor> groupColor;
    std::mt19937 rng(1);
    std::uniform_int_distribution<int32_t> dist(30, 255);

    for (const auto &[group, labelsVec] : groups)
    {
        int32_t r = dist(rng), g = dist(rng), b = dist(rng);
        groupColor[group] = QColor(r, g, b);
    }
    groupColor[0] = QColor(0, 0, 0);

    for (int32_t y = 0; y < height; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int32_t x = 0; x < width; ++x)
        {
            int32_t lbl = labels[y][x];
            int32_t group = labelToGroup.count(lbl) ? labelToGroup[lbl] : 0;
            line[x] = groupColor[group].rgb();
        }
    }
    return result;
}

QImage engine::painter::ImagePainter::recolorLabels(const QImage &image, const engine::binary::BinaryLabels &labels,
                                                    const engine::binary::LabelGroup &targetLabels, QRgb newColor)
{
    QImage result = image.convertToFormat(QImage::Format_RGB32);
    int32_t height = labels.size();
    int32_t width = labels[0].size();

    QSet<int32_t> labelSet(targetLabels.begin(), targetLabels.end());

    for (int32_t y = 0; y < height; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int32_t x = 0; x < width; ++x)
        {
            if (labelSet.contains(labels[y][x]))
            {
                line[x] = newColor;
            }
        }
    }
    return result;
}

QImage engine::painter::ImagePainter::recolorLabel(const QImage &image, const engine::binary::BinaryLabels &labels,
                                                   int32_t targetLabel, QRgb newColor)
{
    QImage result = image.convertToFormat(QImage::Format_RGB32);
    int32_t height = labels.size();
    int32_t width = labels[0].size();

    for (int32_t y = 0; y < height; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int32_t x = 0; x < width; ++x)
        {
            if (labels[y][x] == targetLabel)
            {
                line[x] = newColor;
            }
        }
    }
    return result;
}

QImage engine::painter::ImagePainter::drawLabelsCharacteristics(const QImage &image,
                                                                const engine::binary::LabelesProperties &properties,
                                                                int32_t axisLen, QColor massColor, QColor bboxColor,
                                                                QColor medianColor, QColor axisColor)
{
    QImage result = image.copy();
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);

    for (const auto &property : properties)
    {
        painter.setPen(QPen(massColor, 3));
        painter.drawEllipse(QPointF(property.cx, property.cy), 3, 3);

        painter.setPen(QPen(bboxColor, 3));
        painter.drawRect(QRectF(property.bbox_cx - 3, property.bbox_cy - 3, 6, 6));

        painter.setPen(QPen(medianColor, 3));

        QPolygonF triangle;
        triangle << QPointF(property.median_x, property.median_y - 4)
                 << QPointF(property.median_x - 3, property.median_y + 2)
                 << QPointF(property.median_x + 3, property.median_y + 2);
        painter.drawPolygon(triangle);

        painter.setPen(QPen(axisColor, 2));
        double dx = (axisLen / 2.0) * std::cos(property.orientation);
        double dy = (axisLen / 2.0) * std::sin(property.orientation);
        QPointF p1(property.cx - dx, property.cy - dy);
        QPointF p2(property.cx + dx, property.cy + dy);
        painter.drawLine(p1, p2);
    }
    painter.end();
    return result;
}

QImage engine::painter::ImagePainter::drawBoundingBoxForLabel(const QImage &image,
                                                              const engine::binary::BinaryLabels &labels,
                                                              int32_t targetLabel, QColor boxColor, int32_t thickness)
{
    int height = labels.size();
    int width = (height > 0) ? labels[0].size() : 0;
    int minx = width - 1;
    int miny = height - 1;
    int maxx = 0;
    int maxy = 0;
    bool found = false;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (labels[y][x] == targetLabel)
            {
                minx = std::min(minx, x);
                miny = std::min(miny, y);
                maxx = std::max(maxx, x);
                maxy = std::max(maxy, y);
                found = true;
            }
        }
    }

    QImage result = image.copy();
    if (!found)
    {
        return result;
    }

    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(boxColor, thickness));
    painter.drawRect(QRectF(minx, miny, maxx - minx + 1, maxy - miny + 1));
    painter.end();
    return result;
}

QImage engine::painter::ImagePainter::drawPathByLabels(const QImage &image,
                                                       const engine::searcher::PathToTreasure &path,
                                                       const engine::binary::LabelesProperties &labelsProp,
                                                       QColor pathColor, int32_t thickness)
{
    std::map<int, engine::binary::BinaryRegionProperties> regionMap;
    for (const auto &prop : labelsProp)
    {
        regionMap[prop.label] = prop;
    }

    QImage result = image.copy();
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(pathColor, thickness));

    for (const auto &edge : path)
    {
        int l1 = edge.first;
        int l2 = edge.second;
        auto it1 = regionMap.find(l1);
        auto it2 = regionMap.find(l2);
        if (it1 != regionMap.end() && it2 != regionMap.end())
        {
            const auto &p1 = it1->second;
            const auto &p2 = it2->second;
            painter.drawLine(QPointF(p1.cx, p1.cy), QPointF(p2.cx, p2.cy));
        }
    }

    painter.end();
    return result;
}
