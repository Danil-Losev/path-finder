#include "KernelCreator.h"
#include <qmath.h>
engine::filters::FilterKernel engine::filters::KernelCreator::createGaussianBlurKernel(int32_t size, double sigma)
{
    std::vector<std::vector<double>> kernel(size, std::vector<double>(size));
    double sum = 0.0;
    int32_t halfSize = size / 2;
    for (int32_t i = -halfSize; i <= halfSize; ++i)
    {
        for (int32_t j = -halfSize; j <= halfSize; ++j)
        {
            double value = std::exp(-(i * i + j * j) / (2.0 * sigma * sigma));
            kernel[i + halfSize][j + halfSize] = value;
            sum += value;
        }
    }
    for (auto &row : kernel)
        for (auto &x : row)
            x /= sum;
    return engine::filters::FilterKernel(true, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createMeanKernel(int32_t size)
{
    std::vector<std::vector<double>> kernel(size, std::vector<double>(size, 1.0 / (size * size)));
    return engine::filters::FilterKernel(true, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createSharpenKernel()
{
    std::vector<std::vector<double>> kernel = {
        {0, -1, -1, -1, 0}, {-1, 2, -4, 2, -1}, {-1, -4, 25, -4, -1}, {-1, 2, -4, 2, -1}, {0, -1, -1, -1, 0}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createEdgeDetectionKernel()
{
    std::vector<std::vector<double>> kernel = {
        {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, 24, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createLaplacianKernel()
{
    std::vector<std::vector<double>> kernel = {
        {0, 0, -1, 0, 0}, {0, -1, -2, -1, 0}, {-1, -2, 16, -2, -1}, {0, -1, -2, -1, 0}, {0, 0, -1, 0, 0}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createSobelXKernel()
{
    std::vector<std::vector<double>> kernel = {
        {-2, -1, 0, 1, 2}, {-3, -2, 0, 2, 3}, {-4, -3, 0, 3, 4}, {-3, -2, 0, 2, 3}, {-2, -1, 0, 1, 2}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createSobelYKernel()
{
    std::vector<std::vector<double>> kernel = {
        {-2, -3, -4, -3, -2}, {-1, -2, -3, -2, -1}, {0, 0, 0, 0, 0}, {1, 2, 3, 2, 1}, {2, 3, 4, 3, 2}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createPrewittXKernel()
{
    std::vector<std::vector<double>> kernel = {
        {-2, -1, 0, 1, 2}, {-2, -1, 0, 1, 2}, {-2, -1, 0, 1, 2}, {-2, -1, 0, 1, 2}, {-2, -1, 0, 1, 2}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createPrewittYKernel()
{
    std::vector<std::vector<double>> kernel = {
        {-2, -2, -2, -2, -2}, {-1, -1, -1, -1, -1}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 1}, {2, 2, 2, 2, 2}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createEmbossKernel()
{
    std::vector<std::vector<double>> kernel = {
        {-2, -1, 0, 1, 2}, {-1, 0, 1, 2, 1}, {0, 1, 2, 1, 0}, {1, 2, 1, 0, -1}, {2, 1, 0, -1, -2}};
    return engine::filters::FilterKernel(false, kernel);
}

engine::filters::FilterKernel engine::filters::KernelCreator::createAloKernel(int32_t size, double sigma, double alpha)
{
    std::vector<std::vector<double>> gauss(size, std::vector<double>(size));
    double sum = 0.0;
    int32_t half = size / 2;
    for (int32_t y = -half; y <= half; ++y)
    {
        for (int32_t x = -half; x <= half; ++x)
        {
            double v = std::exp(-(x * x + y * y) / (2.0 * sigma * sigma));
            gauss[y + half][x + half] = v;
            sum += v;
        }
    }
    for (int32_t y = 0; y < size; ++y)
        for (int32_t x = 0; x < size; ++x)
            gauss[y][x] /= sum;

    std::vector<std::vector<double>> identity(size, std::vector<double>(size, 0.0));
    identity[half][half] = 1.0;

    std::vector<std::vector<double>> alo(size, std::vector<double>(size, 0.0));
    for (int32_t y = 0; y < size; ++y)
        for (int32_t x = 0; x < size; ++x)
            alo[y][x] = (1.0 + alpha) * identity[y][x] - alpha * gauss[y][x];

    return FilterKernel(false, alo);
}
