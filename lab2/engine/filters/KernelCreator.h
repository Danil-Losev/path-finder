#ifndef KERNELCREATOR_H
#define KERNELCREATOR_H

#include "structs.h"

namespace engine
{
namespace filters
{
class KernelCreator
{
  public:
    KernelCreator() = delete;
    ~KernelCreator() = delete;
    static engine::filters::FilterKernel createGaussianBlurKernel(int32_t size, double sigma);
    static engine::filters::FilterKernel createSharpenKernel();
    static engine::filters::FilterKernel createEdgeDetectionKernel();
    static engine::filters::FilterKernel createLaplacianKernel();
    static engine::filters::FilterKernel createSobelXKernel();
    static engine::filters::FilterKernel createSobelYKernel();
    static engine::filters::FilterKernel createPrewittXKernel();
    static engine::filters::FilterKernel createPrewittYKernel();
    static engine::filters::FilterKernel createEmbossKernel();
    static engine::filters::FilterKernel createAloKernel(int32_t size, double sigma, double alpha);
    static engine::filters::FilterKernel createMeanKernel(int size);
};
} // namespace filters
} // namespace engine

#endif // KERNELCREATOR_H
