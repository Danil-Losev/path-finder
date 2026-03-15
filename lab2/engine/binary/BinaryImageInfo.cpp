#include "BinaryImageInfo.h"
#include <set>

#include <QMap>

engine::binary::BinaryImageInfo::BinaryImageInfo(QImage binaryImage, double groopingThreshold, double noiseMaxArea)
{
    this->binaryImage = binaryImage;
    this->findLabelConnectedComponents();
    this->analyzeRegions();
    if (noiseMaxArea > 0.0)
    {
        this->filterNoise(noiseMaxArea);
        this->analyzeRegions();
    }
    this->groupByEccentricityAndCompactness(groopingThreshold);
}

constexpr int dx[4] = {1, -1, 0, 0};
constexpr int dy[4] = {0, 0, 1, -1};

void engine::binary::BinaryImageInfo::analyzeRegions()
{
    int height = labels.size();
    if (height == 0)
        return;
    int width = labels[0].size();
    properties.clear();

    std::unordered_map<int, BinaryRegionSum> stats;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int label = labels[y][x];
            if (label == 0)
                continue;
            BinaryRegionSum &regionSum = stats[label];
            regionSum.area += 1;
            regionSum.sx += x;
            regionSum.sy += y;
            regionSum.sxx += x * x;
            regionSum.syy += y * y;
            regionSum.sxy += x * y;
            regionSum.xs.push_back(x);
            regionSum.ys.push_back(y);
            regionSum.minx = std::min(regionSum.minx, x);
            regionSum.maxx = std::max(regionSum.maxx, x);
            regionSum.miny = std::min(regionSum.miny, y);
            regionSum.maxy = std::max(regionSum.maxy, y);

            for (int k = 0; k < 4; ++k)
            {
                int nx = x + dx[k], ny = y + dy[k];
                if (nx < 0 || nx >= width || ny < 0 || ny >= height || labels[ny][nx] != label)
                    regionSum.perimeter += 1;
            }
        }
    }

    for (const auto &kv : stats)
    {
        BinaryRegionProperties r;
        r.label = kv.first;
        const BinaryRegionSum &s = kv.second;

        r.area = s.area;
        r.cx = s.area > 0 ? s.sx / s.area : 0.0;
        r.cy = s.area > 0 ? s.sy / s.area : 0.0;

        r.bbox_cx = (s.minx + s.maxx) / 2.0;
        r.bbox_cy = (s.miny + s.maxy) / 2.0;

        r.median_x = 0;
        r.median_y = 0;
        if (!s.xs.empty() && !s.ys.empty())
        {
            auto xs = s.xs, ys = s.ys;
            std::nth_element(xs.begin(), xs.begin() + xs.size() / 2, xs.end());
            std::nth_element(ys.begin(), ys.begin() + ys.size() / 2, ys.end());
            r.median_x = xs[xs.size() / 2];
            r.median_y = ys[ys.size() / 2];
        }

        r.perimeter = s.perimeter;
        r.compactness = (r.area > 0) ? (double(r.perimeter * r.perimeter) / double(r.area)) : 0.0;

        double mu20 = s.sxx / r.area - r.cx * r.cx;
        double mu02 = s.syy / r.area - r.cy * r.cy;
        double mu11 = s.sxy / r.area - r.cx * r.cy;

        r.orientation = 0.5 * std::atan2(2 * mu11, mu20 - mu02);

        double common = std::sqrt((mu20 - mu02) * (mu20 - mu02) + 4 * mu11 * mu11);
        double lambda1 = 0.5 * (mu20 + mu02 + common);
        double lambda2 = 0.5 * (mu20 + mu02 - common);

        r.eccentricity = 0.0;
        if (lambda1 + lambda2 > 1e-10 && lambda1 > lambda2 && lambda1 > 0.0)
            r.eccentricity = std::sqrt(1.0 - lambda2 / lambda1);

        properties.push_back(r);
    }
}

void engine::binary::BinaryImageInfo::groupByEccentricityAndCompactness(double threshold)
{
    struct Vec2
    {
        double a, b;
    };
    std::vector<Vec2> features;
    for (const auto &p : properties)
    {
        features.push_back({p.eccentricity, p.compactness});
    }

    std::vector<int> groupForIdx(properties.size(), -1);
    int nextGroup = 0;

    for (size_t i = 0; i < properties.size(); ++i)
    {
        if (groupForIdx[i] != -1)
        {
            continue;
        }

        groupForIdx[i] = nextGroup;
        for (size_t j = i + 1; j < properties.size(); ++j)
        {
            if (groupForIdx[j] != -1)
            {
                continue;
            }
            double dist =
                std::sqrt(std::pow(features[i].a - features[j].a, 2.0) + std::pow(features[i].b - features[j].b, 2.0));
            if (dist < threshold)
            {
                groupForIdx[j] = nextGroup;
            }
        }
        ++nextGroup;
    }

    for (size_t i = 0; i < properties.size(); ++i)
    {
        groupedLabels[groupForIdx[i]].push_back(properties[i].label);
    }
}
void engine::binary::BinaryImageInfo::findLabelConnectedComponents()
{
    int32_t width = binaryImage.width();
    int32_t height = binaryImage.height();
    int32_t nextLabel = 1;

    labels.assign(height, std::vector<int32_t>(width, 0));

    std::unordered_map<int32_t, int32_t> labelParent;

    auto find = [&](int32_t label) {
        while (label != labelParent[label])
        {
            label = labelParent[label];
        }
        return label;
    };

    auto unite = [&](int32_t a, int32_t b) {
        int32_t ra = find(a);
        int32_t rb = find(b);
        if (ra != rb)
        {
            labelParent[rb] = ra;
        }
    };

    for (int32_t y = 0; y < height; ++y)
    {
        const uchar *row = binaryImage.constScanLine(y);
        for (int32_t x = 0; x < width; ++x)
        {
            if (row[x] == 0)
            {
                continue;
            }

            std::vector<int32_t> neighborLabels;
            if (x > 0 && row[x - 1] > 0)
            {
                neighborLabels.push_back(labels[y][x - 1]);
            }
            if (y > 0 && binaryImage.constScanLine(y - 1)[x] > 0)
            {
                neighborLabels.push_back(labels[y - 1][x]);
            }

            if (neighborLabels.empty())
            {
                labels[y][x] = nextLabel;
                labelParent[nextLabel] = nextLabel;
                ++nextLabel;
            }
            else
            {
                int32_t smallest = *std::min_element(neighborLabels.begin(), neighborLabels.end());
                labels[y][x] = smallest;
                for (auto l : neighborLabels)
                {
                    unite(smallest, l);
                }
            }
        }
    }

    std::unordered_map<int32_t, int32_t> newLabel;
    int32_t realLabel = 1;
    for (const auto &kv : labelParent)
    {
        int32_t root = find(kv.first);
        if (newLabel.count(root) == 0)
        {
            newLabel[root] = realLabel++;
        }
        newLabel[kv.first] = newLabel[root];
    }

    for (int32_t y = 0; y < height; ++y)
    {
        for (int32_t x = 0; x < width; ++x)
        {
            if (labels[y][x])
            {
                labels[y][x] = newLabel[find(labels[y][x])];
            }
        }
    }
}

void engine::binary::BinaryImageInfo::filterNoise(double maxNoiseArea)
{
    std::set<int> smallLabels;
    for (const auto &p : properties)
    {
        if (p.area <= maxNoiseArea)
        {
            smallLabels.insert(p.label);
        }
    }
    int height = labels.size();
    int width = labels.empty() ? 0 : labels[0].size();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (smallLabels.count(labels[y][x]))
            {
                labels[y][x] = 0;
                if (binaryImage.format() == QImage::Format_Mono || binaryImage.format() == QImage::Format_Grayscale8)
                    binaryImage.setPixel(x, y, 0);
                else
                    binaryImage.setPixelColor(x, y, Qt::black);
            }
        }
    }
}
