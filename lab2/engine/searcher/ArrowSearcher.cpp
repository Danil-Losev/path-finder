#include "ArrowSearcher.h"

int engine::searcher::ArrowSearcher::findRedArrow(const QImage &image, const engine::binary::BinaryLabels &labels)
{
    int height = labels.size(), width = labels[0].size();
    int maxLabel = 0;
    for (const auto &row : labels)
    {
        for (int l : row)
        {
            if (l > maxLabel)
            {
                maxLabel = l;
            }
        }
    }

    std::vector<AreaColorStats> stats(maxLabel + 1);

    for (int y = 0; y < height; ++y)
    {
        const QRgb *srcLine = reinterpret_cast<const QRgb *>(image.constScanLine(y));
        for (int x = 0; x < width; ++x)
        {
            int label = labels[y][x];
            if (label == 0)
            {
                continue;
            }
            QRgb color = srcLine[x];
            stats[label].count++;
            stats[label].sumR += qRed(color);
            stats[label].sumG += qGreen(color);
            stats[label].sumB += qBlue(color);
        }
    }

    int redLabel = 0;
    double bestRedness = 0.0;
    for (int label = 1; label <= maxLabel; ++label)
    {
        if (stats[label].count == 0)
        {
            continue;
        }
        double r = double(stats[label].sumR) / stats[label].count;
        double g = double(stats[label].sumG) / stats[label].count;
        double b = double(stats[label].sumB) / stats[label].count;
        double redness = r - std::max(g, b);
        if (redness > bestRedness && r > 70 && r > 1.0 * std::max(g, b))
        {
            bestRedness = redness;
            redLabel = label;
        }
    }
    return redLabel;
}

engine::binary::LabelGroup engine::searcher::ArrowSearcher::findArrowGroup(
    const engine::binary::GroupedLabels &groupedLabels, int targetLabel)
{
    for (const auto &[groupLabel, group] : groupedLabels)
    {
        if (std::find(group.begin(), group.end(), targetLabel) != group.end())
        {
            return group;
        }
    }
}
