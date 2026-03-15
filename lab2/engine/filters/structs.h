#ifndef STRUCTS_H
#define STRUCTS_H

#include <cstdint>
#include <vector>

namespace engine
{
namespace filters
{
struct FilterKernel
{
    int32_t height;
    int32_t width;
    bool isNormalized = false;
    std::vector<std::vector<double>> kernel;

    FilterKernel(int32_t height, int32_t width, bool isNormalized, std::vector<std::vector<double>> kernel)
        : height(height), width(width), isNormalized(isNormalized), kernel(std::move(kernel))
    {
    }

    FilterKernel(bool isNormalized, std::vector<std::vector<double>> kernel)
        : height(static_cast<int32_t>(kernel.size())),
          width(static_cast<int32_t>(kernel.empty() ? 0 : kernel[0].size())), isNormalized(isNormalized),
          kernel(std::move(kernel))
    {
    }

    FilterKernel() = delete;
    ~FilterKernel() = default;
};
} // namespace filters
} // namespace engine

#endif // STRUCTS_H
