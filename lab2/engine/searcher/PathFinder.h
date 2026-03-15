#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "../binary/BinaryImageInfo.h"
#include <limits>
#include <map>
#include <set>


namespace engine
{
namespace searcher
{
struct TreasureHit
{
    int label = 0;
    double dist = std::numeric_limits<double>::max();
    double hitX = 0;
    double hitY = 0;
};

using PathToTreasure = std::vector<std::pair<int, int>>;
using TreasureAndPath = std::pair<int, PathToTreasure>;

class PathFinder
{
  public:
    PathFinder() = delete;
    ~PathFinder() = delete;

    static TreasureAndPath findArrowPathAndTreasure(const engine::binary::LabelesProperties &regions,
                                                    const engine::binary::BinaryLabels &labels, int startLabel,
                                                    const engine::binary::LabelGroup &arrowLabels);

  private:
    static const engine::binary::BinaryRegionProperties *findNextArrowInDirection(
        const engine::binary::BinaryRegionProperties &curr,
        const std::map<int, engine::binary::BinaryRegionProperties> &regionMap, const std::set<int> &arrowSet,
        const std::set<int> &usedLabels, const engine::binary::BinaryLabels &labels, double direction,
        double maxDist = 1000.0, double step = 0.3, double coneAngle = 0.45);

    static TreasureHit findTreasureAlongDirection(const engine::binary::BinaryRegionProperties &curr,
                                                  const std::set<int> &arrowSet,
                                                  const engine::binary::BinaryLabels &labels, double direction,
                                                  double step = 0.3);
};
} // namespace searcher
} // namespace engine

#endif // PATHFINDER_H
