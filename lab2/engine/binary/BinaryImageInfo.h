#ifndef BINARYIMAGEINFO_H
#define BINARYIMAGEINFO_H

#include <QImage>
#include <vector>

namespace engine
{
namespace binary
{

struct BinaryRegionProperties
{
    int32_t label;             // Номер области
    int32_t area;              // Площадь (число пикселей)
    double cx, cy;             // Центр масс (среднее по x и y)
    double bbox_cx, bbox_cy;   // Центр BBox (центр минимальног�� прямоугольника)
    double median_x, median_y; // Медиана x и y (устойчива к выбросам)
    int32_t perimeter;         // Периметр (число граничных пикселей)
    double compactness;        // Компактность = P^2 / A
    double eccentricity;       // Эксцентриситет (0 — круг, 1 — вытяг.)
    double orientation;        // Угол главной оси в рад, CCW от X, [-pi/2..pi/2]
};

struct BinaryRegionSum
{
    int area = 0, perimeter = 0;
    double sx = 0, sy = 0, sxx = 0, syy = 0, sxy = 0;
    int minx = 1e9, maxx = -1, miny = 1e9, maxy = -1;
    std::vector<int> xs, ys;
};

using BinaryLabels = std::vector<std::vector<int32_t>>;
using LabelesProperties = std::vector<BinaryRegionProperties>;
using LabelGroup = std::vector<int32_t>;
using GroupedLabels = std::unordered_map<int32_t, LabelGroup>;

class BinaryImageInfo
{
  public:
    BinaryImageInfo(QImage binaryImage, double groopingThreshold = 0.12, double noiseMaxArea = 0.0);
    ~BinaryImageInfo() = default;

    QImage getBinaryImage() const
    {
        return binaryImage;
    }
    BinaryLabels getBinaryLabels() const
    {
        return labels;
    }
    LabelesProperties getRegionProperties() const
    {
        return properties;
    }
    GroupedLabels getGroupedLabels() const
    {
        return groupedLabels;
    }

  private:
    QImage binaryImage;
    BinaryLabels labels;
    LabelesProperties properties;
    GroupedLabels groupedLabels;

    void analyzeRegions();
    void groupByEccentricityAndCompactness(double threshold);
    void findLabelConnectedComponents();
    void filterNoise(double maxNoiseArea);
};
} // namespace binary
} // namespace engine

#endif // BINARYIMAGEINFO_H
