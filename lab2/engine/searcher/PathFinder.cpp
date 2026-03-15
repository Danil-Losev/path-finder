#include "PathFinder.h"
#include <qmath.h>

engine::searcher::TreasureAndPath engine::searcher::PathFinder::findArrowPathAndTreasure(
    const engine::binary::LabelesProperties &regions, const engine::binary::BinaryLabels &labels, int startLabel,
    const engine::binary::LabelGroup &arrowLabels)
{
    std::map<int, engine::binary::BinaryRegionProperties> regionMap;
    for (const auto &p : regions)
    {
        regionMap[p.label] = p;
    }
    std::set<int> arrowSet(arrowLabels.begin(), arrowLabels.end());
    auto it = regionMap.find(startLabel);
    if (it == regionMap.end())
    {
        return {0, {}};
    }
    const engine::binary::BinaryRegionProperties *curr = &it->second;
    std::set<int> usedLabels;
    usedLabels.insert(curr->label);
    std::vector<std::pair<int, int>> pathLabels;

    while (true)
    {
        const engine::binary::BinaryRegionProperties *nxtFor =
            findNextArrowInDirection(*curr, regionMap, arrowSet, usedLabels, labels, curr->orientation);
        const engine::binary::BinaryRegionProperties *nxtBack =
            findNextArrowInDirection(*curr, regionMap, arrowSet, usedLabels, labels, curr->orientation + M_PI);

        const engine::binary::BinaryRegionProperties *next = nullptr;
        double dFor = std::numeric_limits<double>::max();
        double dBack = dFor;
        if (nxtFor)
        {
            dFor = std::hypot(nxtFor->cx - curr->cx, nxtFor->cy - curr->cy);
        }
        if (nxtBack)
        {
            dBack = std::hypot(nxtBack->cx - curr->cx, nxtBack->cy - curr->cy);
        }

        if (nxtFor && nxtBack)
        {
            next = (dFor <= dBack) ? nxtFor : nxtBack;
        }
        else if (nxtFor)
        {
            next = nxtFor;
        }
        else if (nxtBack)
        {
            next = nxtBack;
        }
        else
        {
            break;
        }

        pathLabels.push_back({curr->label, next->label});
        usedLabels.insert(next->label);
        curr = next;
    }

    TreasureHit treasureFwd = findTreasureAlongDirection(*curr, arrowSet, labels, curr->orientation);
    TreasureHit treasureBack = findTreasureAlongDirection(*curr, arrowSet, labels, curr->orientation + M_PI);

    TreasureHit bestTreasure;
    if (treasureFwd.label > 0 && treasureBack.label > 0)
    {
        bestTreasure = (treasureFwd.dist <= treasureBack.dist) ? treasureFwd : treasureBack;
    }
    else if (treasureFwd.label > 0)
    {
        bestTreasure = treasureFwd;
    }
    else if (treasureBack.label > 0)
    {
        bestTreasure = treasureBack;
    }

    if (bestTreasure.label > 0)
    {
        pathLabels.push_back({curr->label, bestTreasure.label});
    }

    return {bestTreasure.label, pathLabels};
}

const engine::binary::BinaryRegionProperties *engine::searcher::PathFinder::findNextArrowInDirection(
    const engine::binary::BinaryRegionProperties &curr,
    const std::map<int, engine::binary::BinaryRegionProperties> &regionMap, const std::set<int> &arrowSet,
    const std::set<int> &usedLabels, const engine::binary::BinaryLabels &labels, double direction, double maxDist,
    double step, double coneAngle)
{
    int height = labels.size();
    int width = (height > 0) ? labels[0].size() : 0;
    double x = curr.cx;
    double y = curr.cy;
    double dx = std::cos(direction);
    double dy = std::sin(direction);

    for (double dist = 0.0; dist < maxDist; dist += step)
    {
        x += dx * step;
        y += dy * step;
        int xi = int(std::round(x));
        int yi = int(std::round(y));
        if (xi < 0 || xi >= width || yi < 0 || yi >= height)
        {
            break;
        }
        int label = labels[yi][xi];
        if (label == 0 || label == curr.label)
        {
            continue;
        }
        auto itR = regionMap.find(label);
        if (arrowSet.count(label) && !usedLabels.count(label) && itR != regionMap.end())
        {
            const engine::binary::BinaryRegionProperties &candidate = itR->second;
            double sx = candidate.cx - curr.cx;
            double sy = candidate.cy - curr.cy;
            double d = std::sqrt(sx * sx + sy * sy);
            if (d > 1e-2 && d <= maxDist)
            {
                double vx = dx;
                double vy = dy;
                double dp = (sx * vx + sy * vy) / d;
                if (dp > std::cos(coneAngle))
                {
                    return &candidate;
                }
            }
        }
        else
        {
            return nullptr;
        }
    }
    return nullptr;
}

engine::searcher::TreasureHit engine::searcher::PathFinder::findTreasureAlongDirection(
    const engine::binary::BinaryRegionProperties &curr, const std::set<int> &arrowSet,
    const engine::binary::BinaryLabels &labels, double direction, double step)
{
    int height = labels.size();
    int width = (height > 0) ? labels[0].size() : 0;
    double x = curr.cx;
    double y = curr.cy;
    double dx = std::cos(direction);
    double dy = std::sin(direction);

    for (double d = 0.0; d < std::max(width, height) * 2; d += step)
    {
        x += dx * step;
        y += dy * step;
        int xi = int(std::round(x));
        int yi = int(std::round(y));
        if (xi < 0 || xi >= width || yi < 0 || yi >= height)
        {
            break;
        }
        int l = labels[yi][xi];
        if (l == 0)
        {
            continue;
        }
        if (!arrowSet.count(l))
        {
            double dist2 = std::sqrt((x - curr.cx) * (x - curr.cx) + (y - curr.cy) * (y - curr.cy));
            return TreasureHit{l, dist2, x, y};
        }
    }
    return TreasureHit{};
}
